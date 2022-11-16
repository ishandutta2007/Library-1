#pragma once
#include <bits/stdc++.h>
#include "src/Math/ModIntPrototype.hpp"
/**
 * @title 素数判定
 * @category 数学
 *  O(log N)
 */

// BEGIN CUT HERE
namespace math_internal {
template <class Uint, class mod_pro_t, u64... args>
constexpr bool miller_rabin(Uint n) {
  const mod_pro_t md(n);
  const Uint s = __builtin_ctzll(n - 1), d = n >> s, one = md.set(1),
             n1 = md.norm(md.set(n - 1));
  for (auto a : {args...}) {
    Uint b = a % n, p = pow(md.set(b), d, md), i = s;
    while (p = md.norm(p), (p != one && p != n1 && b && i--)) p = md.mul(p, p);
    if (md.norm(p) != n1 && i != s) return false;
  }
  return true;
}
constexpr bool is_prime(u64 n) {
  if (n < 2 || n % 6 % 4 != 1) return (n | 1) == 3;
  if (n < UINT_MAX) return miller_rabin<u32, MIntPro_Na<u32>, 2, 7, 61>(n);
  if (n < LLONG_MAX)
    return miller_rabin<u64, MIntPro_Montg, 2, 325, 9375, 28178, 450775,
                        9780504, 1795265022>(n);
  return miller_rabin<u64, MIntPro_Na<u64>, 2, 325, 9375, 28178, 450775,
                      9780504, 1795265022>(n);
}
}  // namespace math_internal
using math_internal::is_prime;