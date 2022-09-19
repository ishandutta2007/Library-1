#pragma once
#include <bits/stdc++.h>
/**
 * @title 平方根 $\mathbb{F}_p$
 * @category 数学
 * O(log p)
 */

// BEGIN CUT HERE

constexpr std::int64_t sqrt(std::uint64_t a, std::uint64_t p) {
  if ((a %= p) <= 1 || p == 2) return a;
  using u64 = std::uint64_t;
  auto pow = [&p](u64 x, u64 k) {
    for (u64 ret = 1;; x = (__uint128_t)x * x % p)
      if (k & 1 ? ret = (__uint128_t)ret * x % p : 0; !(k >>= 1)) return ret;
  };
  u64 e = (p - 1) >> 1, b = 0, D = p - a, ret = 1, r2 = 0, b2 = 1;
  if (pow(a, e) != 1) return -1;  // no solution
  while (pow(D, e) == 1) D -= p & -((D += (b++ << 1) | 1) >= p);
  auto mult = [&D, &p](u64 &u1, u64 &u2, u64 v1, u64 v2) {
    u64 tmp = ((__uint128_t)u1 * v1 + (__uint128_t)u2 * v2 % p * D) % p;
    u2 = ((__uint128_t)u1 * v2 + (__uint128_t)u2 * v1) % p;
    u1 = tmp - (p & -(tmp >= p)), u2 -= p & -(u2 >= p);
  };
  for (++e;; mult(b, b2, b, b2)) {
    if (e & 1) mult(ret, r2, b, b2);
    if (!(e >>= 1)) return std::min(ret, p - ret);
  }
}