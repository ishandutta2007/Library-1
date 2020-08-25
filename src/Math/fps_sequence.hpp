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

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Math/FormalPowerSeries.hpp"
#undef call_from_test
#endif

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
  return (a * b).part(N + 1);
}

template <typename mint>
FormalPowerSeries<mint> eulerian(int N) {
  vector<mint> fact(N + 2), finv(N + 2);
  fact[0] = finv[N + 1] = 1;
  for (int i = 1; i <= N + 1; i++) fact[i] = fact[i - 1] * i;
  finv[N + 1] /= fact[N + 1];
  for (int i = N; i >= 0; i--) finv[i] = finv[i + 1] * (i + 1);
  FormalPowerSeries<mint> a(N / 2 + 1), b(N / 2 + 1);
  for (int i = 0; i <= N / 2; i++) {
    a[i] = i & 1 ? -finv[i] * finv[N + 1 - i] : finv[i] * finv[N + 1 - i];
    b[i] = mint(i + 1).pow(N) * fact[N + 1];
  }
  FormalPowerSeries<mint> ret = (a * b).part(N + 1);
  for (int i = 0; i <= N / 2; i++) ret[N - i - 1] = ret[i];
  return ret;
}
