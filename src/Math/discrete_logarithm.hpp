#pragma once
#include <bits/stdc++.h>
#include "src/Math/mod_inv.hpp"
/**
 * @title 離散対数 ($\mathbb{Z}/m\mathbb{Z}$)
 * @category 数学
 * O(√mod)
 */

// BEGIN CUT HERE
int discrete_logarithm(int a, int b, int mod) {
  if (a == 0) return b == 0 ? (mod == 1 ? 0 : 1) : (b == 1 ? 0 : -1);
  auto mul = [&mod](int &x, int y) { x = (std::uint64_t)x * y % mod; };
  int cnt = 0;
  for (int g = 0;; cnt++, b /= g, mod /= g, mul(b, mod_inv<int>(a / g, mod))) {
    if ((b == 1) || (mod == 1)) return cnt;
    if ((g = std::gcd(a, mod)) == 1) break;
    if (b % g != 0) return -1;  // no solution
  }
  int baby = 1, size = 1 << std::__lg(int(std::sqrt(mod)) + 1), mask = size - 1,
      os[size + 1], vs[size][2];
  for (int i = 0; i < size; i++, mul(baby, a)) os[baby & mask]++;
  for (int i = 1; i < size; i++) os[i] += os[i - 1];
  os[size] = size, baby = 1;
  for (int i = 0, j = 0; i < size; i++, mul(baby, a))
    j = --os[baby & mask], vs[j][0] = baby, vs[j][1] = i;
  for (int t = 0, iv = mod_inv<int>(baby, mod); t < mod; t += size, mul(b, iv))
    for (int m = (b & mask), i = os[m + 1]; i-- > os[m];)
      if (b == vs[i][0]) return cnt + vs[i][1] + t;
  return -1;  // no solution
}