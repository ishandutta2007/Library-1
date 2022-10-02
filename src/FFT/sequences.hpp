#pragma once
#include <bits/stdc++.h>
#include "src/FFT/fps_inv.hpp"
#include "src/FFT/fps_exp.hpp"
#include "src/FFT/convolve.hpp"
/**
 * @title 有名な数列(形式的冪級数使用)
 * @category 数学
 * @see
 * https://maspypy.com/%E5%A4%9A%E9%A0%85%E5%BC%8F%E3%83%BB%E5%BD%A2%E5%BC%8F%E7%9A%84%E3%81%B9%E3%81%8D%E7%B4%9A%E6%95%B0-%E9%AB%98%E9%80%9F%E3%81%AB%E8%A8%88%E7%AE%97%E3%81%A7%E3%81%8D%E3%82%8B%E3%82%82%E3%81%AE
 * @see https://en.wikipedia.org/wiki/Bernoulli_number
 * @see https://en.wikipedia.org/wiki/Partition_function_(number_theory)
 * @see https://en.wikipedia.org/wiki/Alternating_permutation
 * @see https://en.wikipedia.org/wiki/Stirling_number
 */

// BEGIN CUT HERE

template <typename mod_t, std::size_t _Nm = 1 << 22>
std::vector<mod_t> bernoulli(int N) {  // O(N log N)
  std::vector<mod_t> ret(N + 1);
  mod_t fact = ret[0] = 1;
  for (int i = 1; i <= N; i++) ret[i] = ret[i - 1] * get_inv<mod_t, _Nm>(i + 1);
  ret = inv<mod_t, _Nm>(ret);
  for (int i = 1; i <= N; fact *= (++i)) ret[i] *= fact;
  return ret;
}

template <typename mod_t, std::size_t _Nm = 1 << 22>
std::vector<mod_t> partition(int N) {  // O(N log N)
  std::vector<mod_t> ret(N + 1, 0);
  for (int k = 1, l = 2; l <= N; l += 6 * k + 7, k += 2) ret[l] = -1;
  for (int k = 2, l = 7; l <= N; l += 6 * k + 7, k += 2) ret[l] = 1;
  for (int k = 1, l = 1; l <= N; l += 6 * k + 5, k += 2) ret[l] = -1;
  for (int k = 2, l = 5; l <= N; l += 6 * k + 5, k += 2) ret[l] = 1;
  return ret[0] = 1, inv<mod_t, _Nm>(ret);
}

template <typename mod_t, std::size_t _Nm = 1 << 22>
std::vector<mod_t> alternating_permutation(int N) {  // O(N log N)
  std::vector<mod_t> f(N + 1), c(N + 1), s(N + 1);
  mod_t fact = f[0] = 1;
  for (int i = 1; i <= N; i++) f[i] = f[i - 1] * get_inv<mod_t, _Nm>(i);
  for (int i = 0; i <= N; i += 4) c[i] = f[i];
  for (int i = 1; i <= N; i += 4) s[i] = f[i];
  for (int i = 2; i <= N; i += 4) c[i] = -f[i];
  for (int i = 3; i <= N; i += 4) s[i] = -f[i];
  s[0] += 1, f = div(s, c);
  for (int i = 1; i <= N; fact *= ++i) f[i] *= fact;
  return f;
}

// enumeration for k = 0,...,K in ∑_{n=0}^{N-1} n^k
template <typename mod_t, std::size_t _Nm = 1 << 22>
std::vector<mod_t> sum_kth_pows(int K, std::uint64_t N) {  // O(K log K)
  std::vector<mod_t> a(K + 1), b(K + 1);
  int i = 1;
  for (a[0] = 1; i <= K; i++) a[i] = a[i - 1] * get_inv<mod_t, _Nm>(i + 1);
  for (b[0] = N, i = 1; i <= K; i++) b[i] = b[i - 1] * N;
  for (i = 0; i <= K; i++) b[i] *= a[i];
  mod_t fact = 1;
  for (a = div(b, a), i = 1; i <= K; fact *= ++i) a[i] *= fact;
  return a;
}

// enumeration for k = 0,...,K in s(N, N-k)
template <typename mod_t, std::size_t _Nm = 1 << 22>
std::vector<mod_t> stirling_first(int K, std::uint64_t N = 0) {  // O(K log K)
  if (N < K) N = K;
  auto a = sum_kth_pows<mod_t, _Nm>(K, N);
  for (int i = 1; i <= K; i++) a[i] *= -get_inv<mod_t, _Nm>(i);
  return a[0] = 0, exp<mod_t, _Nm>(a);
}

// enumeration for k = 0,...,N in S(N, k)
template <typename mod_t, std::size_t _Nm = 1 << 22>
std::vector<mod_t> stirling_second(int N) {  // O(N log N)
  std::vector<mod_t> a(N + 1, 1), b(N + 1);
  for (int i = 1; i <= N; i++) a[i] = a[i - 1] * get_inv<mod_t, _Nm>(i);
  for (int i = 0; i <= N; i += 2) b[i] = a[i];
  for (int i = 1; i <= N; i += 2) b[i] = -a[i];
  for (int i = 0; i <= N; i++) a[i] *= mod_t(i).pow(N);
  return a = convolve<mod_t, _Nm>(a, b), a.resize(N + 1), a;
}