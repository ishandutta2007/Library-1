#pragma once
#include <numeric>
#include <cassert>
#include "src/Math/is_prime.hpp"
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
  auto f= [&md, n, c](Uint x) { return md.plus(md.mul(x, x), c); };
  const Uint m= 1LL << (__lg(n) / 5);
  Uint x= 1, y= md.set(2), z= 1, q= md.set(1), g= 1;
  for (Uint r= 1, i= 0; g == 1; r<<= 1) {
   for (x= y, i= r; i--;) y= f(y);
   for (Uint k= 0; k < r && g == 1; g= gcd(md.get(q), n), k+= m)
    for (z= y, i= min(m, r - k); i--;) y= f(y), q= md.mul(q, md.diff(y, x));
  }
  if (g == n) do {
    z= f(z), g= gcd(md.get(md.diff(z, x)), n);
   } while (g == 1);
  return g;
 }
 static constexpr u64 find_prime_factor(u64 n) {
  if (is_prime(n)) return n;
  for (u64 i= 100; i--;)
   if (n= n < (1 << 30) ? rho<u32, MP_Mo<u32, u64, 32, 31>>(n, i + 1) : n < (1ull << 62) ? rho<u64, MP_Mo<u64, u128, 64, 63>>(n, i + 1) : rho<u64, MP_D2B1>(n, i + 1); is_prime(n)) return n;
  return 0;
 }
 constexpr void init(u64 n) {
  for (u64 p= 2; p < 100 && p * p <= n; p++)
   if (n % p == 0)
    for (dat[sz++].first= p; n % p == 0;) n/= p, ++dat[sz - 1].second;
  for (u64 p= 0; n > 1; dat[sz++].first= p)
   for (p= find_prime_factor(n); n % p == 0;) n/= p, ++dat[sz].second;
 }
public:
 constexpr Factors()= default;
 constexpr Factors(u64 n) { init(n), bubble_sort(dat, dat + sz); }
};
template <class Uint, class MP> constexpr Uint inner_primitive_root(Uint p) {
 const MP md(p);
 const auto f= Factors(p - 1);
 for (Uint ret= 2, one= md.set(1), ng= 0;; ret++) {
  for (auto [q, e]: f)
   if (ng= (md.norm(pow(md.set(ret), (p - 1) / q, md)) == one)) break;
  if (!ng) return ret;
 }
}
constexpr u64 primitive_root(u64 p) {
 if (assert(is_prime(p)); p == 2) return 1;
 if (p < (1 << 30)) return inner_primitive_root<u32, MP_Mo<u32, u64, 32, 31>>(p);
 if (p < (1ull << 62)) return inner_primitive_root<u64, MP_Mo<u64, u128, 64, 63>>(p);
 return inner_primitive_root<u64, MP_D2B1>(p);
}
class Divisors: public ConstexprArray<u64, 110600> {
 constexpr void init(const Factors &f) {
  dat[sz++]= 1;
  for (auto [p, e]: f) {
   u64 pw= p;
   size_t psz= sz;
   for (uint16_t i= 1; i <= e; ++i, pw*= p)
    for (size_t j= 0; j < psz; ++j) dat[sz++]= dat[j] * pw;
  }
 }
public:
 constexpr Divisors()= default;
 constexpr Divisors(const Factors &f) { init(f), bubble_sort(dat, dat + sz); };
 constexpr Divisors(u64 n): Divisors(Factors(n)) {}
};
}  // namespace math_internal
using math_internal::Factors, math_internal::Divisors, math_internal::primitive_root;
constexpr uint64_t totient(const Factors &f) {
 uint64_t ret= 1, i= 0;
 for (auto [p, e]: f)
  for (ret*= p - 1, i= e; --i;) ret*= p;
 return ret;
}
constexpr auto totient(uint64_t n) { return totient(Factors(n)); }