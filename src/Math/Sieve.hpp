#pragma once
#include <bits/stdc++.h>
/**
 * @title 篩など
 * @category 数学
 * 線形篩による素数列挙の前処理
 * 乗法的関数 テーブル列挙 や gcd畳み込みなど
 * @see https://37zigen.com/linear-sieve/
 * @see https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5
 * @see https://en.wikipedia.org/wiki/Dirichlet_convolution
 * @see
 * https://maspypy.com/dirichlet-%E7%A9%8D%E3%81%A8%E3%80%81%E6%95%B0%E8%AB%96%E9%96%A2%E6%95%B0%E3%81%AE%E7%B4%AF%E7%A9%8D%E5%92%8C
 */

// BEGIN CUT HERE

template <int LIM = 1 << 24>
class Sieve {
 public:
  static inline int ps[LIM >> 4], lpf[LIM], lpfe[LIM], lpfpw[LIM], psz = 0;
  static inline void sieve(int N) {  // O(N)
    static int n = 2;
    for (; n <= N; n++) {
      if (!lpf[n]) lpf[n] = ps[psz++] = n;
      for (int j = 0, e = std::min(lpf[n], N / n); j < psz && ps[j] <= e; j++)
        lpf[ps[j] * n] = ps[j];
    }
  }
  static inline void set_lpfe(int N) {  // O(N)
    static int n = 2;
    if (N < n) return;
    sieve(N), std::fill(lpfe + n, lpfe + N + 1, 1);
    for (std::copy(lpf + n, lpf + N + 1, lpfpw + n); n <= N; n++)
      if (int m = n / lpf[n]; lpf[n] == lpf[m])
        lpfe[n] += lpfe[m], lpfpw[n] *= lpfpw[m];
  }

 public:
  static int least_prime_factor(int n) { return sieve(n), lpf[n]; }
  // O(log n)
  static std::map<int, short> factorize(int n) {
    std::map<int, short> ret;
    for (set_lpfe(n); n > 1;) ret[lpf[n]] = lpfe[n], n /= lpfpw[n];
    return ret;
  }
  // O(log n)
  static std::vector<int> divisors(int n) {
    std::vector<int> ret = {1};
    for (auto [p, e] : factorize(n))
      for (std::size_t sz = ret.size(), pw = p; e--; pw *= p)
        for (int i = sz - 1; i >= 0; i--) ret.push_back(ret[i] * pw);
    return std::sort(ret.begin(), ret.end()), ret;
  }
  // O(N)
  static std::vector<int> get_primes(int N) {
    return sieve(N), std::vector<int>(ps, std::upper_bound(ps, ps + psz, N));
  }
  template <class T, class F>
  static inline std::vector<T> completely_multiplicative_table(int N,
                                                               const F &f) {
    std::vector<T> ret(N + 1);
    sieve(N);
    for (int i = 2; i <= N; i++)
      ret[i] = lpf[i] == i ? f(i, 1) : ret[lpf[i]] * ret[i / lpf[i]];
    return ret[1] = 1, ret;
  }
  template <class T, class F>
  static inline std::vector<T> multiplicative_table(int N, const F &f) {
    std::vector<T> ret(N + 1);
    set_lpfe(N);
    for (int i = 2; i <= N; i++)
      ret[i] = lpfpw[i] == i ? f(lpf[i], lpfe[i])
                             : ret[lpfpw[i]] * ret[i / lpfpw[i]];
    return ret[1] = 1, ret;
  }
  // O(N log k / log N + N)
  template <class T>
  static std::vector<T> pow_table(int N, std::uint64_t k) {
    if (k == 0) return std::vector<T>(N + 1, 1);
    auto f = [k](int p, short) {
      T ret = 1, b = p;
      for (auto e = k;; b *= b)
        if (e & 1 ? ret *= b, !(e >>= 1) : !(e >>= 1)) return ret;
    };
    return completely_multiplicative_table<T>(N, f);
  }
  // O(N log MOD / log N + N)
  template <class T>
  static std::vector<T> inv_table(int N) {
    return completely_multiplicative_table<T>(
        N, [](int p, short) { return T(1) / p; });
  }
  // naive , O(N log N)
  template <class T>
  static std::vector<T> dirichlet_conv(const std::vector<T> &a,
                                       const std::vector<T> &b) {
    std::size_t N = std::max(a.size(), b.size()) - 1, i, j;
    std::vector<T> ret(N + 1, 0);
    for (i = a.size(); --i;)
      for (j = std::min(b.size() - 1, N / i); j; j--) ret[i * j] += a[i] * b[j];
    return ret;
  }
  // a is multiplicative, O(N log log N)
  template <class T, class F>
  static std::vector<T> dirichlet_conv(const F &a, std::vector<T> b) {
    std::size_t N = b.size() - 1, j = 0;
    for (sieve(N), b.resize(N + 1, 0); j < psz && ps[j] <= N; j++)
      for (int i = N / ps[j], n, m, e; i; i--)
        for (b[n = ps[j] * i] += a(ps[j], e = 1) * b[m = i]; m % ps[j] == 0;)
          b[n] += a(ps[j], ++e) * b[m /= ps[j]];
    return b;
  }
  // both a and b are multiplicative, O(N)
  template <class T, class F1, class F2>
  static std::vector<T> dirichlet_conv(const F1 &a, const F2 &b,
                                       std::size_t N) {
    auto f = [&a, &b](int p, short e) {
      T ret = a(p, e) + b(p, e);
      for (int k = e; --k;) ret += a(p, e - k) * b(p, k);
      return ret;
    };
    return multiplicative_table<T>(N, f);
  }
  // f -> g s.t. g(n) = sum_{m|n} f(m), O(N log log N)
  template <typename T>
  static void divisor_zeta(std::vector<T> &f) {
    std::size_t N = f.size(), i, j;
    for (sieve(N), i = 0; i < psz && ps[i] < N; i++)
      for (j = 1; ps[i] * j < N; j++) f[ps[i] * j] += f[j];
  }
  // f -> h s.t. f(n) = sum_{m|n} h(m), O(N log log N)
  template <typename T>
  static void divisor_mobius(std::vector<T> &f) {
    std::size_t N = f.size(), i, j;
    for (sieve(N), i = 0; i < psz && ps[i] < N; i++)
      for (j = (N - 1) / ps[i]; j; j--) f[ps[i] * j] -= f[j];
  }
  // O(N log log N)
  template <typename T>
  static std::vector<T> lcm_conv(std::vector<T> a, std::vector<T> b) {
    std::size_t N = std::max(a.size(), b.size());
    a.resize(N), b.resize(N), divisor_zeta(a), divisor_zeta(b);
    for (; N--;) a[N] *= b[N];
    return divisor_mobius(a), a;
  }
  // both a and b are multiplicative, O(N)
  template <class T, class F1, class F2>
  static std::vector<T> lcm_conv(const F1 &a, const F2 &b, std::size_t N) {
    auto f = [&a, &b](int p, short e) {
      T suma = 1, sumb = 1;
      for (short k = 1; k < e; k++) suma += a(p, k), sumb += b(p, k);
      return (a(p, e) + suma) * (b(p, e) + sumb) - suma * sumb;
    };
    return multiplicative_table<T>(N, f);
  }
  // f -> g s.t. g(n) = sum_{n|m} f(m), O(N log log N)
  template <typename T>
  static void multiple_zeta(std::vector<T> &f) {
    std::size_t N = f.size(), i, j;
    for (sieve(N), i = 0; i < psz && ps[i] < N; i++)
      for (j = (N - 1) / ps[i]; j; j--) f[j] += f[ps[i] * j];
  }
  // f -> h s.t. f(n) = sum_{n|m} h(m), O(N log log N)
  template <typename T>
  static void multiple_mobius(std::vector<T> &f) {
    std::size_t N = f.size(), i, j;
    for (sieve(N), i = 0; i < psz && ps[i] < N; i++)
      for (j = 1; ps[i] * j < N; j++) f[j] -= f[ps[i] * j];
  }
  // O(N log log N)
  template <typename T>
  static std::vector<T> gcd_conv(std::vector<T> a, std::vector<T> b) {
    std::size_t N = std::max(a.size(), b.size());
    a.resize(N), b.resize(N), multiple_zeta(a), multiple_zeta(b);
    for (; N--;) a[N] *= b[N];
    return multiple_mobius(a), a;
  }
};