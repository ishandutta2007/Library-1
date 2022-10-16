#pragma once
#include <bits/stdc++.h>
/**
 * @title 素数判定
 * @category 数学
 *  O(log N)
 * constexpr で呼べる
 */

// BEGIN CUT HERE
constexpr std::uint64_t mul(std::uint64_t x, std::uint64_t y, std::uint64_t m) {
  return (__uint128_t)x * y % m;
}
template <std::uint64_t... args>
constexpr bool miller_rabin(std::uint64_t n) {
  const std::uint64_t s = __builtin_ctzll(n - 1), d = n >> s;
  for (auto a : {args...}) {
    std::uint64_t b = a % n, p = 1, i = s;
    for (std::uint64_t k = d, x = b;; x = mul(x, x, n))
      if (k& 1 ? p = mul(p, x, n) : 0; !(k >>= 1)) break;
    while (p != 1 && p != n - 1 && b && i--) p = mul(p, p, n);
    if (p != n - 1 && i != s) return false;
  }
  return true;
}
constexpr bool is_prime(std::uint64_t n) {
  if (n < 2 || n % 6 % 4 != 1) return (n | 1) == 3;
  if (n < UINT_MAX) return miller_rabin<2, 7, 61>(n);
  return miller_rabin<2, 325, 9375, 28178, 450775, 9780504, 1795265022>(n);
}