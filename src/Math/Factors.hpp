#pragma once
#include <bits/stdc++.h>
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
};
class Factors: public ConstexprArray<pair<u64, uint16_t>, 16> {
 template <class Uint, class mod_pro_t> static constexpr Uint rho(Uint n, Uint c) {
  const mod_pro_t md(n);
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
   if (n= n < UINT_MAX ? rho<u32, MP_Na<u32>>(n, i + 1) : n < LLONG_MAX ? rho<u64, MP_Mo>(n, i + 1) : rho<u64, MP_Na<u64>>(n, i + 1); is_prime(n)) return n;
  return 0;
 }
 constexpr void init(u64 n) {
  for (u64 p= 2; p < 100 && p * p <= n; p++)
   if (n % p == 0)
    for (dat[sz++].first= p; n % p == 0;) n/= p, dat[sz - 1].second++;
  for (u64 p= 0; n > 1; dat[sz++].first= p)
   for (p= find_prime_factor(n); n % p == 0;) n/= p, dat[sz].second++;
 }
public:
 constexpr Factors()= default;
 constexpr Factors(u64 n) { init(n), bubble_sort(dat, dat + sz); }
};
template <class Uint, class mod_pro_t> constexpr Uint inner_primitive_root(Uint p) {
 const mod_pro_t md(p);
 const auto f= Factors(p - 1);
 for (Uint ret= 2, one= md.set(1), ng= 0;; ret++) {
  for (auto [q, e]: f)
   if (ng= (md.norm(pow(md.set(ret), (p - 1) / q, md)) == one)) break;
  if (!ng) return ret;
 }
}
constexpr u64 primitive_root(u64 p) {
 if (assert(is_prime(p)); p == 2) return 1;
 if (p < UINT_MAX) return inner_primitive_root<u32, MP_Na<u32>>(p);
 if (p < LLONG_MAX) return inner_primitive_root<u64, MP_Mo>(p);
 return inner_primitive_root<u64, MP_Na<u64>>(p);
}
}  // namespace math_internal
using math_internal::Factors, math_internal::primitive_root;
constexpr std::uint64_t totient(const Factors &f) {
 std::uint64_t ret= 1, i= 0;
 for (const auto &[p, e]: f)
  for (ret*= p - 1, i= e; --i;) ret*= p;
 return ret;
}
constexpr auto totient(std::uint64_t n) { return totient(Factors(n)); }
