#pragma once
#include <bits/stdc++.h>
/**
 * @title 素数判定
 * @category 数学
 *  O(log N)
 * constexpr で呼べる
 */

// BEGIN CUT HERE
constexpr std::uint16_t bsf(std::uint64_t n) {
  constexpr std::uint8_t convert[64] = {
      0,  1,  2,  53, 3,  7,  54, 27, 4,  38, 41, 8,  34, 55, 48, 28,
      62, 5,  39, 46, 44, 42, 22, 9,  24, 35, 59, 56, 49, 18, 29, 11,
      63, 52, 6,  26, 37, 40, 33, 47, 61, 45, 43, 21, 23, 58, 17, 10,
      51, 25, 36, 32, 60, 20, 57, 16, 50, 31, 19, 15, 30, 14, 13, 12};
  return convert[(n & ~(n - 1)) * 157587932685088877 >> 58];
}
constexpr std::uint64_t mul(std::uint64_t x, std::uint64_t y, std::uint64_t m) {
  return (__uint128_t)x * y % m;
}
template <std::uint64_t... args>
constexpr bool miller_rabin(std::uint64_t n) {
  const std::uint64_t s = bsf(n - 1), d = n >> s;
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