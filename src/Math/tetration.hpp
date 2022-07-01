#pragma once
#include <bits/stdc++.h>
#include "src/Math/factorize.hpp"
/**
 * @title テトレーション $H_4(a,b) = a\upuparrows b = a^{a^{\cdot^{\cdot^{a}}}}$
 * @category 数学
 *  O(N^(1/4))
 */
// verify用:
// https://atcoder.jp/contests/summerfes2018-div1/tasks/summerfes2018_f

// BEGIN CUT HERE

constexpr std::uint64_t rec(std::uint64_t a, std::uint64_t b, std::uint64_t m) {
  if (a == 0) return (b ^ 1) & 1;
  if (b == 0 || m == 1) return 1;
  auto mod = [m](__uint128_t x) { return x < m ? x : x % m + m; };
  std::uint64_t ret = 1, e = rec(a, b - 1, carmichael(m));
  for (a = mod(a);; a = mod(__uint128_t(a) * a))
    if (e& 1 ? ret = mod(__uint128_t(ret) * a) : 0; !(e >>= 1)) return ret;
}
constexpr std::uint64_t H4(std::uint64_t a, std::uint64_t b, std::uint64_t m) {
  return (a = rec(a, b, m)) >= m ? a - m : a;
}
