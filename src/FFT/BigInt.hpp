#pragma once
#include <bits/stdc++.h>
#include "src/FFT/NTT.hpp"
class BigInt {
 static constexpr unsigned BASE= 10000000, D= 7;
 using mod_t= StaticModInt<0x3ffffffffa000001>;
 using Vec= std::vector<unsigned>;
 using ntt= NTT<mod_t>;
 bool neg;
 Vec dat;
 BigInt shift(int sz) const { return {neg, Vec(dat.begin() + sz, dat.end())}; }
 BigInt(bool n, const Vec &d): neg(n), dat(d) {}
public:
 BigInt(): neg(false), dat() {}
 BigInt(long long v): neg(v < 0) {
  for (v= std::abs(v); v; v/= BASE) dat.push_back(v % BASE);
 }
 BigInt(const std::string &s): neg(false) {
  int p= 0, x= 0;
  for (; p < (int)s.size() && (s[p] == '-' || s[p] == '+'); p++)
   if (s[p] == '-') neg= !neg;
  for (int i= s.size(), j; i > p; i-= D, dat.push_back(x), x= 0)
   for (j= std::max(p, i - int(D)); j < i;) x= x * 10 + s[j++] - '0';
  shrink();
 }
 inline void shrink() {
  while (!dat.empty() && !dat.back()) dat.pop_back();
  if (dat.empty()) neg= false;
 }
 std::string to_str() const {
  if (is_zero()) return "0";
  std::stringstream ss;
  if (neg) ss << '-';
  ss << (dat.empty() ? 0 : dat.back());
  for (long long i= dat.size() - 1; i-- > 0;) ss << std::setw(D) << std::setfill('0') << dat[i];
  std::string ret;
  return ss >> ret, ret;
 }
 bool is_zero() const { return dat.empty() || (dat.size() == 1 && !dat[0]); }
 bool operator<(const BigInt &r) const {
  if (neg != r.neg) return neg;
  if (dat.size() != r.dat.size()) return (dat.size() < r.dat.size()) ^ neg;
  for (int i= dat.size(); i--;)
   if (dat[i] != r.dat[i]) return (dat[i] < r.dat[i]) ^ neg;
  return false;
 }
 bool operator>(const BigInt &r) const { return r < *this; }
 bool operator<=(const BigInt &r) const { return !(r < *this); }
 bool operator>=(const BigInt &r) const { return !(*this < r); }
 bool operator==(const BigInt &r) const { return (neg == r.neg && dat == r.dat) || (is_zero() && r.is_zero()); }
 bool operator!=(const BigInt &r) const { return !(*this == r); }
 BigInt abs() const { return BigInt(false, dat); }
 BigInt operator-() const { return BigInt(!neg, dat); }
 BigInt operator+(const BigInt &r) const {
  if (neg != r.neg) return *this - (-r);
  auto [ret, tmp]= dat.size() > r.dat.size() ? std::make_pair(*this, &r) : std::make_pair(r, this);
  int car= 0, i, n= ret.dat.size(), m= tmp->dat.size();
  for (i= 0; i < m; i++) ret.dat[i]-= BASE & -(car= ((ret.dat[i]+= car + tmp->dat[i]) >= BASE));
  if (car) {
   while (i < n && ret.dat[i] == BASE - 1) ret.dat[i++]= 0;
   i < n ? ret.dat[i]++ : (ret.dat.push_back(1), 0);
  }
  return ret;
 }
 BigInt operator-(const BigInt &r) const {
  if (neg != r.neg) return *this + (-r);
  if (r.is_zero()) return *this;
  if (is_zero()) return -r;
  auto [ret, tmp]= abs() > r.abs() ? std::make_pair(*this, &r) : std::make_pair(r, this);
  int car= 0, i, n= ret.dat.size(), m= tmp->dat.size();
  for (i= 0; i < m; i++) ret.dat[i]+= BASE & -(car= ((ret.dat[i]-= car + tmp->dat[i]) >> 31));
  while (car && i < n && !ret.dat[i]) ret.dat[i++]= BASE - 1;
  return ret.neg^= (tmp == this), ret.dat[i]-= car, ret.shrink(), ret;
 }
 long long operator%(long long r) const {
  long long ret= 0;
  for (int i= dat.size(); i--;) ret= (ret * BASE + dat[i]) % r;
  return ret;
 }
 BigInt operator*(const BigInt &r) const {
  if (is_zero() || r.is_zero()) return 0;
  const int n= dat.size(), m= r.dat.size(), sz= n + m - 1;
  static mod_t f[1 << 20], g[1 << 20], f2[1 << 17][16], g2[1 << 17][16];
  static long long h[1 << 20];
  if (int i= n, j; std::min(n, m) >= 74) {
   const int rl= get_len(sz), l= get_len(std::max(n, m));
   const int fl= std::pow(l, 0.535) * 8.288;
   if (l + fl < sz && sz <= (rl >> 3) * 5) {
    const int l= rl >> 4, l2= l << 1, nn= (n + l - 1) / l, mm= (m + l - 1) / l, ss= nn + mm - 1;
    for (int k= i= 0, s; k < n; i++, k+= l) {
     for (j= s= std::min(l, n - k); j--;) f2[i][j]= dat[k + j];
     std::fill_n(f2[i] + s, l2 - s, mod_t()), ntt::dft(l2, f2[i]);
    }
    if (this != &r)
     for (int k= i= 0, s; k < m; i++, k+= l) {
      for (j= s= std::min(l, m - k); j--;) g2[i][j]= dat[k + j];
      std::fill_n(g2[i] + s, l2 - s, mod_t()), ntt::dft(l2, g2[i]);
     }
    else
     for (i= nn; i--;) std::copy_n(f2[i], l2, g2[i]);
    for (i= l2; i--;) f[i]= f2[0][i] * g2[0][i];
    for (ntt::idft(l2, f), i= l2; i--;) h[i]= f[i].val();
    for (int k= l, ed, ii= 1; ii < ss; ++ii, k+= l) {
     j= std::max(0, ii - nn + 1), ed= std::min(mm - 1, ii);
     for (i= l2; i--;) f[i]= f2[ii - ed][i] * g2[ed][i];
     for (; j < ed; ++j)
      for (i= l2; i--;) f[i]+= f2[ii - j][i] * g2[j][i];
     for (ntt::idft(l2, f), i= std::min(l, sz - k); i--;) h[i + k]+= f[i].val();
     for (i= std::min(l2, sz - k); i-- > l;) h[i + k]= f[i].val();
    }
   } else {
    const int len= sz <= l + fl ? l : get_len(sz);
    for (i= n; i--;) f[i]= dat[i];
    std::fill_n(f + n, len - n, mod_t()), ntt::dft(len, f);
    if (this != &r) {
     for (i= m; i--;) g[i]= r.dat[i];
     std::fill_n(g + m, len - m, mod_t()), ntt::dft(len, g);
     for (i= len; i--;) f[i]*= g[i];
    } else
     for (i= len; i--;) f[i]*= f[i];
    for (ntt::idft(len, f), i= len; i < sz; f[i - len]-= h[i], i++)
     for (h[i]= 0, j= i - m + 1; j < n; j++) h[i]+= (long long)dat[j] * r.dat[i - j];
    for (i= std::min(sz, len); i--;) h[i]= f[i].val();
   }
  } else
   for (std::fill_n(h, sz, 0); i--;)
    for (j= m; j--;) h[i + j]+= (long long)dat[i] * r.dat[j];
  BigInt ret(neg ^ r.neg, Vec(sz));
  long long car= 0;
  for (int i= 0; i < sz; i++, car/= BASE) ret.dat[i]= (car+= h[i]) % BASE;
  for (; car; car/= BASE) ret.dat.emplace_back(car % BASE);
  return ret;
 }
 BigInt operator/(const BigInt &r) const {
  assert(!r.is_zero());
  if (r.dat.size() == 1 && r.dat.back() == 1) return r.neg ? -*this : *this;
  BigInt a= this->abs(), b= r.abs();
  if (a < b) return 0;
  const int pb= dat.size(), qb= r.dat.size(), prec= std::max(pb - qb, 1);
  int l= std::min(prec, 3), ql= std::min(qb, 6), nl, nql;
  BigInt x(0, Vec(l + 1)), p, rr= b.shift(qb - ql), c(0, Vec(l + ql + 1));
  x.dat.back()= 1, c.dat.back()= 2;
  while (x != p) p.dat.swap(x.dat), x= (p * (c - rr * p)).shift(l + ql);
  if (l != prec)
   for (p.neg= true; x != p; l= nl, ql= nql) {
    nl= std::min(l * 2 + 1, prec), nql= std::min(ql * 2 + 1, qb);
    p.dat.swap(x.dat), x= (p * (c - rr * p)).shift(2 * l - nl + ql);
    if (p.neg= false; nql != ql) rr= b.shift(qb - nql);
    c.dat.back()= 0, c.dat.resize(nql + nl + 1), c.dat.back()= 2;
   }
  if (x= (x * a).shift(pb + (pb == qb)); a >= (x + 1) * b) x+= 1;
  return x.neg= neg ^ r.neg, x;
 }
 BigInt operator%(const BigInt &r) const { return *this - (*this / r) * r; }
 BigInt &operator+=(const BigInt &r) { return *this= *this + r; }
 BigInt &operator-=(const BigInt &r) { return *this= *this - r; }
 BigInt &operator*=(const BigInt &r) { return *this= *this * r; }
 BigInt &operator/=(const BigInt &r) { return *this= *this / r; }
 BigInt &operator%=(const BigInt &r) { return *this= *this % r; }
 friend std::istream &operator>>(std::istream &is, BigInt &v) {
  std::string s;
  return is >> s, v= BigInt(s), is;
 }
 friend std::ostream &operator<<(std::ostream &os, const BigInt &v) { return os << v.to_str(), os; }
};