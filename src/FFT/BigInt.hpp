#pragma once
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include "src/FFT/NTT.hpp"
namespace math_internal {
class BigInt {
 static constexpr u64 BASE= 1e15;
 static constexpr int8_t D= 15;
 using Vec= vector<u64>;
 bool neg;
 Vec dat;
 BigInt(bool n, const Vec &d): neg(n), dat(d) {}
public:
 BigInt(): neg(false), dat() {}
 BigInt(__int128_t v): neg(v < 0) {
  for (v= v < 0 ? -v : v; v; v/= BASE) dat.push_back(v % BASE);
 }
 BigInt(const string &s): neg(false) {
  int p= 0;
  u64 x= 0;
  for (; p < (int)s.size() && (s[p] == '-' || s[p] == '+'); ++p)
   if (s[p] == '-') neg= !neg;
  for (int i= s.size(), j; i > p; i-= D, dat.push_back(x), x= 0)
   for (j= max(p, i - D); j < i;) x= x * 10 + s[j++] - '0';
  shrink();
 }
 inline void shrink() {
  while (!dat.empty() && !dat.back()) dat.pop_back();
  if (dat.empty()) neg= false;
 }
 string to_str() const {
  if (is_zero()) return "0";
  stringstream ss;
  if (neg) ss << '-';
  ss << (dat.empty() ? 0 : dat.back());
  for (int i= dat.size() - 1; i-- > 0;) ss << setw(D) << setfill('0') << dat[i];
  string ret;
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
  auto [ret, tmp]= dat.size() > r.dat.size() ? make_pair(*this, &r) : make_pair(r, this);
  int car= 0, i, n= ret.dat.size(), m= tmp->dat.size();
  for (i= 0; i < m; i++) ret.dat[i]-= BASE & -(car= ((ret.dat[i]+= car + tmp->dat[i]) >= BASE));
  if (car) {
   while (i < n && ret.dat[i] == BASE - 1) ret.dat[i++]= 0;
   i < n ? ++ret.dat[i] : (ret.dat.push_back(1), 0);
  }
  return ret;
 }
 BigInt operator-(const BigInt &r) const {
  if (neg != r.neg) return *this + (-r);
  if (r.is_zero()) return *this;
  if (is_zero()) return -r;
  auto [ret, tmp]= abs() > r.abs() ? make_pair(*this, &r) : make_pair(r, this);
  int car= 0, i, n= ret.dat.size(), m= tmp->dat.size();
  for (i= 0; i < m; i++) ret.dat[i]+= BASE & -(car= ((ret.dat[i]-= car + tmp->dat[i]) >> 63));
  while (car && i < n && !ret.dat[i]) ret.dat[i++]= BASE - 1;
  return ret.neg^= (tmp == this), ret.dat[i]-= car, ret.shrink(), ret;
 }
 long long operator%(long long r) const {
  long long ret= 0;
  for (int i= dat.size(); i--;) ret= ((u128)ret * BASE + dat[i]) % r;
  return ret;
 }
 BigInt operator*(const BigInt &r) const {
  using mint1= ModInt<MOD1>;
  using mint2= ModInt<MOD2>;
  using mint3= ModInt<MOD3>;
  using mint4= ModInt<MOD4>;
  using ntt1= NTT<mint1>;
  using ntt2= NTT<mint2>;
  using ntt3= NTT<mint3>;
  using ntt4= NTT<mint4>;
  static constexpr mint2 iv21= mint2(1) / MOD1;
  static constexpr mint3 iv32= mint3(1) / MOD2, iv31= iv32 / MOD1;
  static constexpr mint4 iv43= mint4(1) / MOD3, iv42= iv43 / MOD2, iv41= iv42 / MOD1;
  if (is_zero() || r.is_zero()) return 0;
  const int n= dat.size(), m= r.dat.size(), sz= n + m - 1;
  static mint1 f1[1 << 20], g1[1 << 20];
  static mint2 f2[1 << 20], g2[1 << 20];
  static mint3 f3[1 << 20], g3[1 << 20];
  static mint4 f4[1 << 20], g4[1 << 20];
  static u128 h[1 << 20];
  if (int i= n, j; min(n, m) >= 135) {
   const int l= pw2(max(n, m)), fl= std::pow(l, 0.535) * 8.288, len= sz <= l + fl ? l : pw2(sz);
   for (i= n; i--;) f1[i]= dat[i];
   for (i= n; i--;) f2[i]= dat[i];
   for (i= n; i--;) f3[i]= dat[i];
   for (i= n; i--;) f4[i]= dat[i];
   fill_n(f1 + n, len - n, mint1()), ntt1::dft(len, f1), fill_n(f2 + n, len - n, mint2()), ntt2::dft(len, f2), fill_n(f3 + n, len - n, mint3()), ntt3::dft(len, f3), fill_n(f4 + n, len - n, mint4()), ntt4::dft(len, f4);
   if (this != &r) {
#define TMP(k) \
 for (i= m; i--;) g##k[i]= r.dat[i]; \
 fill_n(g##k + m, len - m, mint##k()), ntt##k::dft(len, g##k); \
 for (i= len; i--;) f##k[i]*= g##k[i];
    TMP(1) TMP(2) TMP(3) TMP(4)
#undef TMP
   } else {
    for (i= len; i--;) f1[i]*= f1[i];
    for (i= len; i--;) f2[i]*= f2[i];
    for (i= len; i--;) f3[i]*= f3[i];
    for (i= len; i--;) f4[i]*= f4[i];
   }
   for (ntt1::idft(len, f1), ntt2::idft(len, f2), ntt3::idft(len, f3), ntt4::idft(len, f4), i= len; i < sz; f1[i - len]-= h[i], f2[i - len]-= h[i], f3[i - len]-= h[i], f4[i - len]-= h[i], ++i)
    for (h[i]= 0, j= i - m + 1; j < n; j++) h[i]+= (u128)dat[j] * r.dat[i - j];
   for (i= min(sz, len); i--;) {
    u32 r1= f1[i].val(), r2= (iv21 * (f2[i] - r1)).val(), r3= (iv31 * (f3[i] - r1) - iv32 * r2).val();
    h[i]= ((u128)((u64)(iv41 * (f4[i] - r1) - iv42 * r2 - iv43 * r3).val() * MOD3 + r3) * MOD2 + r2) * MOD1 + r1;
   }
  } else
   for (fill_n(h, sz, 0); i--;)
    for (j= m; j--;) h[i + j]+= (u128)dat[i] * r.dat[j];
  BigInt ret(neg ^ r.neg, Vec(sz));
  u128 car= 0;
  for (int i= 0; i < sz; ++i, car/= BASE) ret.dat[i]= (car+= h[i]) % BASE;
  for (; car; car/= BASE) ret.dat.emplace_back(car % BASE);
  return ret;
 }
 BigInt operator/(const BigInt &r) const {
  assert(!r.is_zero());
  BigInt a= this->abs(), b= r.abs();
  if (a < b) return 0;
  const u64 norm= BASE / (b.dat.back() + 1);
  const int s= (a*= norm).dat.size(), t= (b*= norm).dat.size(), deg= s - t + 2;
  const u64 yb= b.dat.back();
  int k= deg;
  while (k > 64) k= (k + 1) / 2;
  BigInt z(0, Vec(k + 2)), rem(0, Vec(t));
  rem.dat.back()= 1;
  for (int i= z.dat.size(); i--;) {
   if (rem.dat.size() == t) {
    if (b <= rem) z.dat[i]= 1, rem-= b;
   } else if (rem.dat.size() > t) {
    u64 q= ((u128)rem.dat[rem.dat.size() - 1] * BASE + rem.dat[rem.dat.size() - 2]) / yb;
    BigInt yq= b * q;
    while (rem < yq) --q, yq-= b;
    rem-= yq, z.dat[i]= q;
   }
   if (i) rem.dat.insert(rem.dat.begin(), 0);
  }
  for (z.shrink(); k < deg; k<<= 1) {
   int d= min(t, 2 * k + 1);
   BigInt x= z * z, w2= z + z;
   Vec w_(k + 1);
   x.dat.insert(x.dat.begin(), 0), x*= BigInt(0, Vec(b.dat.end() - d, b.dat.end())), x.dat.erase(x.dat.begin(), x.dat.begin() + d), copy(w2.dat.begin(), w2.dat.end(), back_inserter(w_)), z= BigInt(0, w_) - x, z.dat.erase(z.dat.begin());
  }
  z.dat.erase(z.dat.begin(), z.dat.begin() + k - deg);
  BigInt q= a * z;
  return q.dat.erase(q.dat.begin(), q.dat.begin() + t + deg), q.shrink(), q.neg= neg ^ r.neg, q;
 }
 BigInt operator%(const BigInt &r) const { return *this - (*this / r) * r; }
 BigInt &operator+=(const BigInt &r) { return *this= *this + r; }
 BigInt &operator-=(const BigInt &r) { return *this= *this - r; }
 BigInt &operator*=(const BigInt &r) { return *this= *this * r; }
 BigInt &operator/=(const BigInt &r) { return *this= *this / r; }
 BigInt &operator%=(const BigInt &r) { return *this= *this % r; }
 friend istream &operator>>(istream &is, BigInt &v) {
  string s;
  return is >> s, v= BigInt(s), is;
 }
 friend ostream &operator<<(ostream &os, const BigInt &v) { return os << v.to_str(), os; }
 explicit operator int() { return is_zero() ? 0 : neg ? -dat[0] : dat[0]; }
};
}
using math_internal::BigInt;