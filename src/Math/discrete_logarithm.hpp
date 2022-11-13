#pragma once
#include <bits/stdc++.h>
#include "src/Math/mod_inv.hpp"
/**
 * @title 離散対数 ($\mathbb{Z}/m\mathbb{Z}$)
 * @category 数学
 * O(√mod)
 */

// BEGIN CUT HERE
constexpr int discrete_logarithm(unsigned a, unsigned b, unsigned mod) {
  if (a == 0) return b == 0 ? (mod == 1 ? 0 : 1) : (b == 1 ? 0 : -1);
  using u32 = unsigned;
  auto mul = [&mod](u32 &x, u32 y) { x = (std::uint64_t)x * y % mod; };
  u32 cnt = 0;
  for (u32 g = 0;; cnt++, b /= g, mod /= g, mul(b, mod_inv<int>(a / g, mod))) {
    if ((b == 1) || (mod == 1)) return cnt;
    if ((g = std::gcd(a, mod)) == 1) break;
    if (b % g != 0) return -1;  // no solution
  }
  u32 baby = 1, size = 1 << std::__lg(int(std::sqrt(mod))), mask = size - 1,
      os[size + 1] = {}, vs[size][2] = {};
  for (u32 i = 0; i < size; i++, mul(baby, a)) os[baby & mask]++;
  for (u32 i = 1; i < size; i++) os[i] += os[i - 1];
  os[size] = size, baby = 1;
  for (u32 i = 0, j = 0; i < size; i++, mul(baby, a))
    j = --os[baby & mask], vs[j][0] = baby, vs[j][1] = i;
  for (u32 t = 0, iv = mod_inv<int>(baby, mod); t < mod; t += size, mul(b, iv))
    for (u32 m = (b & mask), i = os[m + 1]; i-- > os[m];)
      if (b == vs[i][0]) return cnt + vs[i][1] + t;
  return -1;  // no solution
}