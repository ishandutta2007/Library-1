#pragma once
#include <bits/stdc++.h>
#include "src/Math/Factors.hpp"
/**
 * @title テトレーション $H_4(a,b) = a^{a^{\cdot^{\cdot^{a}}}}$
 * $\mathbb{Z}/m\mathbb{Z}$
 * @category 数学
 *  O(m^(1/4))
 */
// verify用:
// https://atcoder.jp/contests/summerfes2018-div1/tasks/summerfes2018_f

// BEGIN CUT HERE
constexpr std::uint64_t rec(std::uint64_t a, std::uint64_t b, std::uint64_t m) {
  if (a == 0) return (b ^ 1) & 1;
  if (b == 0 || m == 1) return 1;
  std::uint64_t ret = 1, k = 1, tmp = 1, i = 0;
  for (const auto &[p, e] : Factors(m)) {
    for (tmp = p - 1, i = e - (p == 2 && e > 3); --i;) tmp *= p;
    k = std::lcm(k, tmp);
  }
  auto mod = [m](__uint128_t x) { return x < m ? x : x % m + m; };
  for (k = rec(a, b - 1, k), a = mod(a);; a = mod(__uint128_t(a) * a))
    if (k & 1 ? ret = mod(__uint128_t(ret) * a) : 0; !(k >>= 1)) return ret;
}
constexpr std::uint64_t H4(std::uint64_t a, std::uint64_t b, std::uint64_t m) {
  return (a = rec(a, b, m)) >= m ? a - m : a;
}
