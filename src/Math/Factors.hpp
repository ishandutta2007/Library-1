#pragma once
#include <bits/stdc++.h>
#include "src/Math/is_prime.hpp"
/**
 * @title 高速素因数分解など
 * @category 数学
 *  O(N^(1/4))
 * constexpr で呼べる
 */

// BEGIN CUT HERE
template <class T>
constexpr void bubble_sort(T *bg, T *ed) {
  for (int sz = ed - bg, i = 0; i < sz; i++)
    for (int j = sz; --j > i;)
      if (auto tmp = bg[j - 1]; bg[j - 1] > bg[j])
        bg[j - 1] = bg[j], bg[j] = tmp;
}
template <class T, std::size_t _Nm>
struct ConstexprArray {
  constexpr std::size_t size() const { return sz; }
  constexpr auto &operator[](int i) const { return dat[i]; }
  constexpr auto *begin() const { return dat; }
  constexpr auto *end() const { return dat + sz; }

 protected:
  T dat[_Nm] = {};
  std::size_t sz = 0;
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
      if (n % p == 0)
        for (dat[sz++].first = p; n % p == 0;) n /= p, dat[sz - 1].second++;
    for (std::uint64_t p = 0; n > 1; dat[sz++].first = p)
      for (p = find_prime_factor(n); n % p == 0;) n /= p, dat[sz].second++;
  }

 public:
  constexpr Factors() = default;
  constexpr Factors(std::uint64_t n) { init(n), bubble_sort(dat, dat + sz); }
};
constexpr std::uint64_t totient(const Factors &f) {
  std::uint64_t ret = 1, i = 0;
  for (const auto &[p, e] : f)
    for (ret *= p - 1, i = e; --i;) ret *= p;
  return ret;
}
constexpr auto totient(std::uint64_t n) { return totient(Factors(n)); }

constexpr std::uint64_t primitive_root(std::uint64_t p) {
  if (assert(is_prime(p)); p == 2) return 1;
  auto f = Factors(p - 1);
  for (std::uint64_t ret = 2, pw = 0, x = 0, k = 0, ng = 0;; ret++) {
    for (auto [q, e] : f) {
      for (pw = 1, x = ret, k = (p - 1) / q;; x = mul(x, x, p))
        if (k & 1 ? pw = mul(pw, x, p) : 0; !(k >>= 1)) break;
      if (ng = (pw == 1)) break;
    }
    if (!ng) return ret;
  }
}