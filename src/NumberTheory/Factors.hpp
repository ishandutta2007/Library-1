#pragma once
#include <numeric>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <vector>
#include "src/NumberTheory/is_prime.hpp"
#include "src/Math/binary_gcd.hpp"
namespace math_internal {
template <class T> constexpr void bubble_sort(T *bg, T *ed) {
 for (int sz= ed - bg, i= 0; i < sz; i++)
  for (int j= sz; --j > i;)
   if (auto tmp= bg[j - 1]; bg[j - 1] > bg[j]) bg[j - 1]= bg[j], bg[j]= tmp;
}
template <class T, size_t _Nm> struct ConstexprArray {
 constexpr size_t size() const { return sz; }
 constexpr auto &operator[](int i) const { return dat[i]; }
 constexpr auto *begin() const { return dat; }
 constexpr auto *end() const { return dat + sz; }
protected:
 T dat[_Nm]= {};
 size_t sz= 0;
 friend ostream &operator<<(ostream &os, const ConstexprArray &r) {
  os << "[";
  for (size_t i= 0; i < r.sz; ++i) os << r[i] << ",]"[i == r.sz - 1];
  return os;
 }
};
class Factors: public ConstexprArray<pair<u64, uint16_t>, 16> {
 template <class Uint, class MP> static constexpr Uint rho(Uint n, Uint c) {
  const MP md(n);
  auto f= [&md, c](Uint x) { return md.plus(md.mul(x, x), c); };
  const Uint m= 1LL << (__lg(n) / 5);
  Uint x= 1, y= md.set(2), z= 1, q= md.set(1), g= 1;
  for (Uint r= 1, i= 0; g == 1; r<<= 1) {
   for (x= y, i= r; i--;) y= f(y);
   for (Uint k= 0; k < r && g == 1; g= binary_gcd<Uint>(md.get(q), n), k+= m)
    for (z= y, i= min(m, r - k); i--;) y= f(y), q= md.mul(q, md.diff(y, x));
  }
  if (g == n) do {
    z= f(z), g= binary_gcd<Uint>(md.get(md.diff(z, x)), n);
   } while (g == 1);
  return g;
 }
 static constexpr u64 find_prime_factor(u64 n) {
  if (is_prime(n)) return n;
  for (u64 i= 100; i--;)
   if (n= n < (1 << 30) ? rho<u32, MP_Mo32>(n, i + 1) : n < (1ull << 62) ? rho<u64, MP_Mo64>(n, i + 1) : n < (1ull << 62) ? rho<u64, MP_D2B1_1>(n, i + 1) : rho<u64, MP_D2B1_2>(n, i + 1); is_prime(n)) return n;
  return 0;
 }
 constexpr void init(u64 n) {
  for (u64 p= 2; p < 98 && p * p <= n; ++p)
   if (n % p == 0)
    for (dat[sz++].first= p; n % p == 0;) n/= p, ++dat[sz - 1].second;
  for (u64 p= 0; n > 1; dat[sz++].first= p)
   for (p= find_prime_factor(n); n % p == 0;) n/= p, ++dat[sz].second;
 }
public:
 constexpr Factors()= default;
 constexpr Factors(u64 n) { init(n), bubble_sort(dat, dat + sz); }
};
}
using math_internal::Factors;
constexpr uint64_t totient(const Factors &f) {
 uint64_t ret= 1, i= 0;
 for (auto [p, e]: f)
  for (ret*= p - 1, i= e; --i;) ret*= p;
 return ret;
}
constexpr auto totient(uint64_t n) { return totient(Factors(n)); }
template <class Uint= uint64_t> std::vector<Uint> enumerate_divisors(const Factors &f) {
 int k= 1;
 for (auto [p, e]: f) k*= e + 1;
 std::vector<Uint> ret(k, 1);
 k= 1;
 for (auto [p, e]: f) {
  int sz= k;
  for (Uint pw= 1; pw*= p, e--;)
   for (int j= 0; j < sz;) ret[k++]= ret[j++] * pw;
 }
 return ret;
}
template <class Uint> std::vector<Uint> enumerate_divisors(Uint n) { return enumerate_divisors<Uint>(Factors(n)); }
