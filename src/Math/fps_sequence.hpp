#pragma once
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/FormalPowerSeries.hpp"
/**
 * @title 数列(形式的冪級数使用)
 * @category 数学
 *  O(NlogN)
 * @see https://min-25.hatenablog.com/entry/2015/04/07/160154
 * @see https://en.wikipedia.org/wiki/Bernoulli_number
 * @see https://en.wikipedia.org/wiki/Partition_function_(number_theory)
 * @see https://en.wikipedia.org/wiki/Stirling_number
 * @see https://en.m.wikipedia.org/wiki/Eulerian_number
 */

// BEGIN CUT HERE

template <typename mint>
FormalPowerSeries<mint> bernoulli(int N) {
  FormalPowerSeries<mint> ret(N + 1);
  ret[0] = 1;
  for (int i = 1; i <= N; i++) ret[i] = ret[i - 1] / mint(i + 1);
  ret = ret.inv();
  mint fact = 1;
  for (int i = 1; i <= N; fact *= (++i)) ret[i] *= fact;
  return ret;
}

template <typename mint>
FormalPowerSeries<mint> partition(int N) {
  FormalPowerSeries<mint> ret(N + 1);
  ret[0] = 1;
  for (int k = 1; 1ll * k * (3 * k + 1) / 2 <= N; k++)
    ret[k * (3 * k + 1) / 2] = (k & 1 ? -1 : 1);
  for (int k = 1; 1ll * k * (3 * k - 1) / 2 <= N; k++)
    ret[k * (3 * k - 1) / 2] = (k & 1 ? -1 : 1);
  return ret.inv();
}

template <typename mint>
FormalPowerSeries<mint> stirling_first(int N) {
  if (!N) return {1};
  auto ret = stirling_first<mint>(N / 2);
  ret *= ret.shift(N / 2);
  if (N & 1) ret *= {N - 1, 1};
  return ret;
}

template <typename mint>
FormalPowerSeries<mint> stirling_second(int N) {
  FormalPowerSeries<mint> a(N + 1), b(N + 1);
  mint finv = 1;
  for (int i = 0; i <= N; finv /= (++i))
    a[i] = mint(i).pow(N) * finv, b[i] = i & 1 ? -finv : finv;
  auto ret = a * b;
  return ret.resize(N + 1), ret;
}
