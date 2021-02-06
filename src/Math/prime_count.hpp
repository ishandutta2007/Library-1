#pragma once
#include <bits/stdc++.h>
/**
 * @title 素数カウントなど
 * 乗法的関数や加法的関数の和もあり
 * @category 数学
 */

// verify用:
// https://atcoder.jp/contests/abc172/tasks/abc172_d
// https://atcoder.jp/contests/xmascon19/tasks/xmascon19_d
// https://atcoder.jp/contests/xmascon19/tasks/xmascon19_e (加法的関数)

// BEGIN CUT HERE

// O(d^2√N+dN^(3/4)/log N) d := degre of polynomial
template <class T = __int128_t>
auto polynomial_prime_sum_table(std::uint64_t N, const std::vector<T> &poly) {
  const int sqrtN = std::sqrt(N), d = poly.size();
  std::vector<T> primes, small(sqrtN + 1, 0), large(sqrtN + 1, 0);
  std::vector<std::vector<T>> s(d, std::vector<T>(sqrtN + 1)),
      l(d, std::vector<T>(sqrtN + 1));
  for (int n = 1, k = 0; n <= sqrtN; n++, k = 0)
    for (T prd = n; k < d; prd *= (n + ++k)) s[k][n] = prd / (k + 1);
  for (int n = 1, k = 0; n <= sqrtN; n++, k = 0)
    for (T prd = N / n; k < d; prd *= ((N / n) + ++k)) l[k][n] = prd / (k + 1);
  if (d > 2) {
    std::vector<T> stir(d, 0);
    stir[1] = 1;
    for (int k = 2; k < d; stir[k++] = 1) {
      for (int j = k - 1; j; j--) stir[j] = stir[j - 1] + stir[j] * (k - 1);
      for (int n = 1; n <= sqrtN; n++)
        for (int j = 1; j < k; j++)
          s[k][n] -= stir[j] * s[j][n], l[k][n] -= stir[j] * l[j][n];
    }
  }
  for (int k = 0; k < d; k++)
    for (int n = 1; n <= sqrtN; n++) s[k][n] -= 1, l[k][n] -= 1;
  for (int p = 2, k = 0; p <= sqrtN; p++, k = 0)
    if (s[0][p] > s[0][p - 1]) {
      primes.emplace_back(p);
      std::uint64_t q = std::uint64_t(p) * p, M = N / p;
      int t = sqrtN / p, u = min<std::uint64_t>(sqrtN, N / q);
      for (T pw = 1; k < d; k++, pw *= p)
        if (!k || poly[k] != T(0)) {
          T tk = s[k][p - 1];
          for (int i = 1; i <= t; i++) l[k][i] -= (l[k][i * p] - tk) * pw;
          for (int i = t + 1; i <= u; i++)
            l[k][i] -= (s[k][double(M) / i] - tk) * pw;
          for (int i = sqrtN; (std::uint64_t)i >= q; i--)
            s[k][i] -= (s[k][double(i) / p] - tk) * pw;
        }
    }
  for (int n = 1; n <= sqrtN; n++)
    for (int k = 0; k < d; k++)
      small[n] += s[k][n] * poly[k], large[n] += l[k][n] * poly[k];
  return std::make_tuple(primes, small, large);
}

auto prime_count_table(std::uint64_t N) {
  return polynomial_prime_sum_table<std::uint64_t>(N, {1});
}

std::uint64_t prime_count(std::uint64_t N) {
  return std::get<2>(prime_count_table(N))[1];
}

template <class T>
T polynomial_prime_sum(std::uint64_t N, const std::vector<T> &poly) {
  return std::get<2>(polynomial_prime_sum_table<T>(N, poly))[1];
}

template <class T, class F>
T additive_sum(std::uint64_t N, F f, std::vector<T> poly) {
  const std::uint64_t sqrtN = std::sqrt(N);
  auto [primes, s, l] = polynomial_prime_sum_table<T>(N, poly);
  T ret = l[1];
  for (std::uint64_t d = 2, nN = N / d, nd; nN; nN = N / (d = nd))
    ret += (nN > sqrtN ? l[d] : s[nN]) * ((nd = N / nN + 1) - d);
  for (std::uint64_t p : primes)
    for (std::uint64_t pw = p * p, e = 2; pw <= N; e++, pw *= p)
      ret += (f(p, e) - f(p, e - 1)) * (N / pw);
  return ret;
}

template <class T = __int128_t, class F>
T multiplicative_sum(std::uint64_t N, const F &f, const std::vector<T> &poly) {
  const std::uint64_t sqrtN = std::sqrt(N);
  auto [primes, s, l] = polynomial_prime_sum_table<T>(N, poly);
  for (auto it = primes.rbegin(); it != primes.rend(); it++) {
    std::uint64_t p = *it, M = N / p, q = p * p;
    int t = sqrtN / p, u = min(sqrtN, N / q);
    T tk = s[p - 1];
    for (auto i = q; i <= sqrtN; i++) s[i] += (s[double(i) / p] - tk) * f(p, 1);
    for (int i = u; i > t; i--) l[i] += (s[double(M) / i] - tk) * f(p, 1);
    for (int i = t; i >= 1; i--) l[i] += (l[i * p] - tk) * f(p, 1);
  }
  for (auto n = sqrtN; n; n--) s[n] += 1, l[n] += 1;
  auto dfs = [&](auto rc, std::uint64_t n, std::size_t bg, T cf) -> T {
    if (cf == T(0)) return T(0);
    T ret = cf * (n > sqrtN ? l[double(N) / n] : s[n]);
    for (auto i = bg; i < primes.size(); i++) {
      std::uint64_t p = primes[i], q = p * p, nn = double(n) / q;
      if (!nn) break;
      for (int e = 2; nn; nn = double(nn) / p, e++)
        ret += rc(rc, nn, i + 1, cf * (f(p, e) - f(p, 1) * f(p, e - 1)));
    }
    return ret;
  };
  return dfs(dfs, N, 0, 1);
}
