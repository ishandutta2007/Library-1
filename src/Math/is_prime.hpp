#pragma once
#include <bits/stdc++.h>
#include "src/Math/ModIntPrototype.hpp"
namespace math_internal {
template <class Uint, class mod_pro_t, u64... args> constexpr bool miller_rabin(Uint n) {
 const mod_pro_t md(n);
 const Uint s= __builtin_ctzll(n - 1), d= n >> s, one= md.set(1), n1= md.norm(md.set(n - 1));
 for (auto a: {args...})
  if (Uint b= a % n; b)
   if (Uint p= md.norm(pow(md.set(b), d, md)); p != one)
    for (int i= s; p != n1; p= md.norm(md.mul(p, p)))
     if (!(--i)) return 0;
 return 1;
}
constexpr bool is_prime(u64 n) {
 if (n < 2 || n % 6 % 4 != 1) return (n | 1) == 3;
 if (n < UINT_MAX) return miller_rabin<u32, MP_Na<u32>, 2, 7, 61>(n);
 if (n < (LLONG_MAX >> 1)) return miller_rabin<u64, MP_Mo, 2, 325, 9375, 28178, 450775, 9780504, 1795265022>(n);
 return miller_rabin<u64, MP_Na<u64>, 2, 325, 9375, 28178, 450775, 9780504, 1795265022>(n);
}
}
using math_internal::is_prime;