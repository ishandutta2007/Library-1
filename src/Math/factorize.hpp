#pragma once
#include <bits/stdc++.h>
/**
 * @title 高速素因数分解
 * @category 数学
 *  O(N^(1/4))
 * ついでに原始根探索
 */

// BEGIN CUT HERE

bool is_prime(std::uint64_t n) {
  if (n < 2 || n % 6 % 4 != 1) return (n | 1) == 3;
  __uint128_t s = __builtin_ctzll(n - 1), d = n >> s, p, i, e, b;
  for (auto a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
    for (p = 1, i = s, e = d, b = a % n; e; e >>= 1, b = b * b % n)
      if (e & 1) p = p * b % n;
    while (p != 1 && p != n - 1 && a % n && i--) p = p * p % n;
    if (p != n - 1 && i != s) return false;
  }
  return true;
}

std::vector<std::uint64_t> factorize(std::uint64_t n) {
  std::vector<std::uint64_t> ret;
  std::queue<std::uint64_t> que;
  for (que.push(n); !que.empty();) {
    __uint128_t m = que.front(), x = 0, y = 0, t = 0, prd = 2, i = 1, q;
    que.pop();
    if (m == 1) continue;
    if (!is_prime(m)) {
      auto f = [&](__uint128_t a) { return a * a % m + 1; };
      while (t++ % 40 || std::gcd<std::uint64_t, std::uint64_t>(prd, m) == 1) {
        if (x == y) x = ++i, y = f(x);
        if ((q = prd * (std::max(x, y) - std::min(x, y)) % m)) prd = q;
        x = f(x), y = f(f(y));
      }
      auto a = std::gcd<std::uint64_t, std::uint64_t>(prd, m);
      que.push(a), que.push(m / a);
    } else
      ret.push_back(m);
  }
  return std::sort(ret.begin(), ret.end()), ret;
}

std::map<std::uint64_t, short> factorize_map(std::uint64_t n) {
  std::map<std::uint64_t, short> ret;
  for (auto &p : factorize(n)) ret[p]++;
  return ret;
}

std::vector<std::uint64_t> divisors(std::uint64_t n) {
  std::vector<std::uint64_t> ret = {1};
  for (auto [p, e] : factorize_map(n))
    for (std::uint64_t sz = ret.size(), pw = p; e--; pw *= p)
      for (int i = sz - 1; i >= 0; i--) ret.push_back(ret[i] * pw);
  return std::sort(ret.begin(), ret.end()), ret;
}

std::uint64_t primitive_root(std::uint64_t p) {
  assert(is_prime(p));
  auto divs = divisors(p - 1);
  __uint128_t g = 2, pw, e, b;
  for (bool isok = true;; g++, isok = true) {
    for (int i = divs.size() - 2; i && isok; i--, isok &= pw != 1)
      for (pw = 1, e = divs[i], b = g; e; e >>= 1, b = b * b % p)
        if (e & 1) pw = pw * b % p;
    if (isok) return g;
  }
}
