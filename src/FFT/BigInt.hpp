#pragma once
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include "src/FFT/NTT.hpp"
class BigInt {
 static constexpr unsigned BASE= 1000000, D= 6;
 using mod_t= ModInt<0x3ffffffffa000001>;
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
  static mod_t f[1 << 20], g[1 << 20];
  static long long h[1 << 20];
  if (int i= n, j; std::min(n, m) >= 74) {
   const int rl= pw2(sz), l= pw2(std::max(n, m)), fl= std::pow(l, 0.535) * 8.288, len= sz <= l + fl ? l : pw2(sz);
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
  BigInt a= this->abs(), b= r.abs();
  if (a < b) return 0;
  const int norm= BASE / (b.dat.back() + 1), s= (a*= norm).dat.size(), t= (b*= norm).dat.size(), deg= s - t + 2, yb= b.dat.back();
  int k= deg;
  while (k > 64) k= (k + 1) / 2;
  BigInt z(0, Vec(k + 2)), rem(0, Vec(t));
  rem.dat.back()= 1;
  for (int i= z.dat.size(); i--;) {
   if (rem.dat.size() == t) {
    if (b <= rem) z.dat[i]= 1, rem-= b;
   } else if (rem.dat.size() > t) {
    int q= ((long long)rem.dat[rem.dat.size() - 1] * BASE + rem.dat[rem.dat.size() - 2]) / yb;
    BigInt yq= b * q;
    while (rem < yq) --q, yq-= b;
    for (rem-= yq; b <= rem;) ++q, rem-= b;
    z.dat[i]= q;
   }
   if (i) rem.dat.insert(rem.dat.begin(), 0);
  }
  for (z.shrink(); k < deg; k<<= 1) {
   int d= std::min(t, 2 * k + 1);
   BigInt x= z * z, w2= z + z;
   Vec w_(k + 1);
   x.dat.insert(x.dat.begin(), 0), x*= BigInt(0, Vec(b.dat.end() - d, b.dat.end())), x.dat.erase(x.dat.begin(), x.dat.begin() + d), std::copy(w2.dat.begin(), w2.dat.end(), std::back_inserter(w_)), z= BigInt(0, w_) - x, z.dat.erase(z.dat.begin());
  }
  z.dat.erase(z.dat.begin(), z.dat.begin() + k - deg);
  BigInt q= a * z;
  for (q.dat.erase(q.dat.begin(), q.dat.begin() + t + deg), z= b * q; a < z;) q-= 1, z-= b;
  for (rem= a - z; b <= rem;) q+= 1, rem-= b;
  return q.shrink(), q.neg= neg ^ r.neg, q;
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
 explicit operator int() { return is_zero() ? 0 : neg ? -dat[0] : dat[0]; }
};