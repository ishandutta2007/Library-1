#pragma once
#include <cassert>
#include "src/NumberTheory/is_prime.hpp"
namespace math_internal {
template <class Int, class MP> constexpr i64 inner_sqrt(Int a, Int p) {
 const MP md(p);
 Int e= (p - 1) >> 1, one= md.set(1);
 if (a= md.set(a); md.norm(pow(a, e, md)) != one) return -1;
 Int b= 0, d= md.diff(0, a), ret= one, r2= 0, b2= one;
 while (md.norm(pow(d, e, md)) == one) b= md.plus(b, one), d= md.diff(md.mul(b, b), a);
 auto mult= [&md, d](Int &u1, Int &u2, Int v1, Int v2) {
  Int tmp= md.plus(md.mul(u1, v1), md.mul(md.mul(u2, v2), d));
  u2= md.plus(md.mul(u1, v2), md.mul(u2, v1)), u1= tmp;
 };
 for (++e;; mult(b, b2, b, b2)) {
  if (e & 1) mult(ret, r2, b, b2);
  if (!(e>>= 1)) return ret= md.get(ret), ret * 2 < p ? ret : p - ret;
 }
}
}
constexpr long long mod_sqrt(long long a, long long p) {
 assert(p > 0), assert(a >= 0), assert(is_prime(p)), a%= p;
 if (a <= 1 || p == 2) return a;
 if (p < (1 << 30)) return math_internal::inner_sqrt<unsigned, math_internal::MP_Mo32>(a, p);
 if (p < (1ll << 62)) return math_internal::inner_sqrt<unsigned long long, math_internal::MP_Mo64>(a, p);
 return math_internal::inner_sqrt<unsigned long long, math_internal::MP_D2B1_1>(a, p);
}