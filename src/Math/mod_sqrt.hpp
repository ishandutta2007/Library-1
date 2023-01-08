#pragma once
#include <bits/stdc++.h>
#include "src/Math/is_prime.hpp"
namespace math_internal {
template <class Int, class mod_pro_t> constexpr Int inner_sqrt(Int a, Int p) {
 const mod_pro_t md(p);
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
constexpr int64_t mod_sqrt(int64_t a, int64_t p) {
 assert(p > 0), assert(a >= 0), assert(is_prime(p)), a%= p;
 if (a <= 1 || p == 2) return a;
 if (p < INT_MAX) return inner_sqrt<int, MP_Na<u32>>(a, p);
 return inner_sqrt<int64_t, MP_Mo>(a, p);
}
}
using math_internal::mod_sqrt;