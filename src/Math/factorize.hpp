#pragma once
#include <bits/stdc++.h>
/**
 * @title 高速素因数分解など
 * @category 数学
 *  O(N^(1/4))
 * constexpr で呼べる
 */

// BEGIN CUT HERE

#ifndef MATH_TEMP
#define MATH_TEMP
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
constexpr std::uint64_t pow(std::uint64_t x, std::uint64_t k, std::uint64_t m) {
  for (std::uint64_t ret = 1;; x = mul(x, x, m))
    if (k & 1 ? ret = mul(ret, x, m) : 0; !(k >>= 1)) return ret;
}
template <std::uint64_t... args>
constexpr bool miller_rabin(std::uint64_t n) {
  const std::uint64_t s = bsf(n - 1), d = n >> s;
  for (auto a : {args...}) {
    std::uint64_t b = a % n, p = pow(b, d, n), i = s;
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
template <class T>
constexpr void bubble_sort(T *bg, T *ed) {
  for (int sz = ed - bg, i = 0; i < sz; i++)
    for (int j = sz; --j > i;)
      if (auto tmp = bg[j - 1]; bg[j - 1] > bg[j])
        bg[j - 1] = bg[j], bg[j] = tmp;
}

template <class T, std::size_t _Nm>
struct ConstexprArray {
  constexpr std::size_t size() const { return _sz; }
  constexpr auto &operator[](int i) const { return dat[i]; }
  constexpr auto *begin() const { return dat; }
  constexpr auto *end() const { return dat + _sz; }

 protected:
  T dat[_Nm] = {};
  std::size_t _sz = 0;
};
class Factors
    : public ConstexprArray<std::pair<std::uint64_t, std::uint16_t>, 16> {
  static constexpr std::uint64_t rho(std::uint64_t n, std::uint64_t c) {
    auto f = [n, c](std::uint64_t x) { return ((__uint128_t)x * x + c) % n; };
    std::uint64_t x = 1, y = 2, z = 1, q = 1, g = 1;
    const std::uint64_t m = 1LL << (std::__lg(n) / 5);
    for (std::uint64_t r = 1, i = 0; g == 1; r <<= 1) {
      for (x = y, i = r; i--;) y = f(y);
      for (std::uint64_t k = 0; k < r && g == 1; g = std::gcd(q, n), k += m)
        for (z = y, i = std::min(m, r - k); i--;)
          y = f(y), q = mul(q, (x > y ? x - y : y - x), n);
    }
    if (g == n) do {
        z = f(z), g = std::gcd((x > z ? x - z : z - x), n);
      } while (g == 1);
    return g;
  }
  static constexpr std::uint64_t find_prime_factor(std::uint64_t n) {
    if (is_prime(n)) return n;
    for (std::uint64_t i = 100, m = 0; i--; n = m)
      if (m = rho(n, i + 1); is_prime(m)) return m;
    return 0;
  }
  constexpr void init(std::uint64_t n) {
    for (std::uint64_t p = 2; p < 100 && p * p <= n; p++)
      if (n % p == 0) {
        for (; n % p == 0;) n /= p, dat[_sz].second++;
        dat[_sz++].first = p;
      }
    for (std::uint64_t p = 0; n > 1; dat[_sz++].first = p)
      for (p = find_prime_factor(n); n % p == 0;) n /= p, dat[_sz].second++;
    bubble_sort(dat, dat + _sz);
  }

 public:
  constexpr Factors() = default;
  constexpr Factors(std::uint64_t n) { init(n); }
};
constexpr std::uint64_t totient(const Factors &f) {
  std::uint64_t ret = 1, i = 0;
  for (const auto &[p, e] : f)
    for (ret *= p - 1, i = e; --i;) ret *= p;
  return ret;
}
constexpr auto totient(std::uint64_t n) { return totient(Factors(n)); }
constexpr std::uint64_t carmichael(const Factors &f) {
  std::uint64_t ret = 1, i = 0, tmp = 1;
  for (const auto &[p, e] : f) {
    for (tmp = p - 1, i = e - (p == 2 && e > 2); --i;) tmp *= p;
    ret = std::lcm(ret, tmp);
  }
  return ret;
}
constexpr auto carmichael(std::uint64_t n) { return carmichael(Factors(n)); }
#endif