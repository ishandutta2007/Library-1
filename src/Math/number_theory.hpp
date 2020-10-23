#pragma once
#include <bits/stdc++.h>
/**
 * @title 数論
 * @category 数学
 *  約数や倍数についてのゼータ変換やメビウス変換
 *  をもとに数論的関数やgcd畳み込みなどを実現
 *  O(N log log N)
 * @see https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5
 * @see https://en.wikipedia.org/wiki/Dirichlet_convolution
 */

// verify用:
// https://atcoder.jp/contests/agc038/tasks/agc038_c

// BEGIN CUT HERE

namespace number_theory {
std::vector<int> primes;
const int MAX_N = 1 << 24;
int mpf[MAX_N];  // minimum prime factor
void init(int n) {
  primes.push_back(2);
  for (int i = 2; i <= n; i += 2) mpf[i] = 2;
  for (long long p = 3; p <= n; p += 2)
    if (!mpf[p]) {
      mpf[p] = p;
      primes.push_back(p);
      for (long long i = p * p; i <= n; i += 2 * p)
        if (!mpf[i]) mpf[i] = p;
    }
}

std::vector<pair<int, int>> prime_factorize(int n) {  // O(log n)
  std::vector<pair<int, int>> res;
  while (n > 1) {
    int p = mpf[n];
    int e = 0;
    while (mpf[n] == p) e++, n /= p;
    res.push_back(make_pair(p, e));
  }
  return res;
}

// f -> g s.t. g(n) = sum_{m|n} f(m)
template <typename T>
void divisor_zeta(std::vector<T> &f) {
  int n = f.size();
  if (!primes.size()) init(n);
  for (int p : primes) {
    if (p > n) break;
    for (int i = 1; p * i < n; i++) f[p * i] += f[i];
  }
}
// f -> h s.t. f(n) = sum_{m|n} h(m)
template <typename T>
void divisor_mobius(std::vector<T> &f) {
  int n = f.size();
  if (!primes.size()) init(n);
  for (int p : primes) {
    if (p > n) break;
    for (int i = (n - 1) / p; i > 0; i--) f[p * i] -= f[i];
  }
}
// get table of Mobius function
template <typename T = int>
std::vector<T> get_mu(int n) {
  std::vector<T> f(n + 1, T(0));
  f[1] = T(1);
  divisor_mobius(f);
  return f;
}
// get table of Euler's totient function
template <typename T = int>
std::vector<T> get_phi(int n) {
  std::vector<T> f(n + 1);
  std::iota(f.begin(), f.end(), 0);
  divisor_mobius(f);
  return f;
}
// get table of number-of-divisors function
template <typename T = int>
std::vector<T> get_d(int n) {
  std::vector<T> f(n + 1, 1);
  divisor_zeta(f);
  return f;
}
// get table of sum-of-divisors function
template <typename T = int>
std::vector<T> get_sigma(int n) {
  std::vector<T> f(n + 1);
  std::iota(f.begin(), f.end(), 0);
  divisor_zeta(f);
  return f;
}
template <typename T>
static std::vector<T> lcm_convolution(std::vector<T> a, std::vector<T> b) {
  int n = max(a.size(), b.size());
  a.resize(n);
  b.resize(n);
  divisor_zeta(a);
  divisor_zeta(b);
  for (int i = 0; i < n; i++) a[i] *= b[i];
  divisor_mobius(a);
  return a;
}
// f -> g s.t. g(n) = sum_{n|m} f(m)
template <typename T>
static void multiple_zeta(std::vector<T> &f) {
  int n = f.size();
  if (!primes.size()) init(n);
  for (int p : primes) {
    if (p > n) break;
    for (int i = (n - 1) / p; i > 0; i--) f[i] += f[p * i];
  }
}
// f -> h s.t. f(n) = sum_{n|m} h(m)
template <typename T>
static void multiple_mobius(std::vector<T> &f) {
  int n = f.size();
  if (!primes.size()) init(n);
  for (int p : primes) {
    if (p > n) break;
    for (int i = 1; p * i < n; i++) f[i] -= f[p * i];
  }
}
template <typename T>
static std::vector<T> gcd_convolution(std::vector<T> a, std::vector<T> b) {
  int n = std::max(a.size(), b.size());
  a.resize(n);
  b.resize(n);
  multiple_zeta(a);
  multiple_zeta(b);
  for (int i = 0; i < n; i++) a[i] *= b[i];
  multiple_mobius(a);
  return a;
}
}  // namespace number_theory
