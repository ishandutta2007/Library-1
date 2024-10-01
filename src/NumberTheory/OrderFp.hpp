#pragma once
#include <array>
#include "src/NumberTheory/Factors.hpp"
namespace math_internal {
class OrderFp {
 u64 p;
 std::array<u64, 17> prod;
 template <class Uint, class MP> constexpr Uint p_rt() const {
  const MP md(p);
  for (Uint ret= 2, one= md.set(1), ng= 0, m= p - 1;; ++ret) {
   Uint a= md.set(ret);
   for (auto [q, e]: factors)
    if ((ng= (md.norm(pow(a, m / q, md)) == one))) break;
   if (!ng) return ret;
  }
 }
 template <class Uint, class MP> constexpr Uint ord_(u8 l, u8 r, Uint x, const MP &md) const {
  Uint ret= 1;
  if (r - l == 1) {
   Uint one= md.set(1);
   auto [q, e]= factors[l];
   for (u8 i= e; i--; ret*= q, x= pow(x, q, md))
    if (x == one) break;
   return ret;
  }
  u8 m= (l + r) / 2;
  return ord_(l, m, pow(x, prod[r] / prod[m], md), md) * ord_(m, r, pow(x, prod[m] / prod[l], md), md);
 }
 template <class Uint, class MP> constexpr Uint ord(Uint x) const {
  const MP md(p);
  return ord_(0, factors.size(), md.set(x), md);
 }
public:
 Factors factors;
 constexpr OrderFp(u64 p): p(p), prod({1}), factors(p - 1) {
  assert(is_prime(p));
  for (u8 i= 0, d= factors.size(); i < d; ++i) {
   auto [q, e]= factors[i];
   prod[i + 1]= prod[i];
   for (u8 j= e; j--;) prod[i + 1]*= q;
  }
 }
 constexpr u64 primitive_root() const {
  if (p == 2) return 1;
  if (p < (1 << 30)) return p_rt<u32, MP_Mo>();
  if (p < (1ull << 62)) return p_rt<u64, MP_Mo>();
  if (p < (1ull << 63)) return p_rt<u64, MP_D2B1_1>();
  return p_rt<u64, MP_D2B1_2>();
 }
 constexpr u64 operator()(u64 x) const {
  if (x%= p; !x) return 0;
  if (x == 1) return 1;
  if (p < (1 << 30)) return ord<u32, MP_Mo>(x);
  if (p < (1ull << 62)) return ord<u64, MP_Mo>(x);
  if (p < (1ull << 63)) return ord<u64, MP_D2B1_1>(x);
  return ord<u64, MP_D2B1_2>(x);
 }
};
}
using math_internal::OrderFp;