#pragma once
#include <bits/stdc++.h>
/**
 * @title 離散対数 ($\mathbb{Z}/m\mathbb{Z}$)
 * @category 数学
 * O(√mod)
 */

// BEGIN CUT HERE
int discrete_log(unsigned a, unsigned b, unsigned mod) {
  if (a == 0) return b == 0 ? (mod == 1 ? 0 : 1) : (b == 1 ? 0 : -1);
  using u32 = unsigned;
  auto mul = [&mod](u32 &x, u32 y) { x = (std::uint64_t)x * y % mod; };
  auto inv = [&mod](u32 s) -> u32 {
    int x = 1, y = 0, t = mod;
    for (int q, z, u; t;)
      z = x, u = s, x = y, y = z - y * (q = s / t), s = t, t = u - t * q;
    return x < 0 ? mod - (-x) % mod : x % mod;
  };
  u32 cnt = 0;
  for (u32 g;; cnt++, b /= g, mod /= g, mul(b, inv(a / g))) {
    if ((b == 1) || (mod == 1)) return cnt;
    if ((g = std::gcd(a, mod)) == 1) break;
    if (b % g != 0) return -1;  // no solution
  }
  u32 mid = std::ceil(std::sqrt(mod)), baby = 1;
  std::unordered_map<u32, u32> mp;
  for (u32 i = 0; i < mid; i++, mul(baby, a)) mp.insert({baby, i});
  for (u32 i = 0, iv = inv(baby), giant = b; i < mid; mul(giant, iv), i++)
    if (auto it = mp.find(giant); it != mp.end())
      return cnt + i * mid + it->second;
  return -1;  // no solution
}
