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

template <typename Modint>
FormalPowerSeries<Modint> bernoulli(int N) {
  FormalPowerSeries<Modint> ret(N + 1);
  ret[0] = 1;
  for (int i = 1; i <= N; i++) ret[i] = ret[i - 1] / Modint(i + 1);
  ret = ret.inv();
  Modint fact = 1;
  for (int i = 1; i <= N; fact *= (++i)) ret[i] *= fact;
  return ret;
}

template <typename Modint>
FormalPowerSeries<Modint> partition(int N) {
  FormalPowerSeries<Modint> ret(N + 1);
  ret[0] = 1;
  for (int k = 1; 1ll * k * (3 * k + 1) / 2 <= N; k++)
    ret[k * (3 * k + 1) / 2] = (k & 1 ? -1 : 1);
  for (int k = 1; 1ll * k * (3 * k - 1) / 2 <= N; k++)
    ret[k * (3 * k - 1) / 2] = (k & 1 ? -1 : 1);
  return ret.inv();
}

template <typename Modint>
FormalPowerSeries<Modint> stirling_first(int N) {
  if (!N) return {1};
  auto ret = stirling_first<Modint>(N / 2);
  ret *= ret.shift(N / 2);
  if (N & 1) ret *= {N - 1, 1};
  return ret;
}

template <typename Modint>
FormalPowerSeries<Modint> stirling_second(int N) {
  FormalPowerSeries<Modint> a(N + 1), b(N + 1);
  Modint finv = 1;
  for (int i = 0; i <= N; finv /= (++i))
    a[i] = Modint(i).pow(N) * finv, b[i] = i & 1 ? -finv : finv;
  return (a * b).part(N + 1);
}

template <typename Modint>
FormalPowerSeries<Modint> eulerian(int N) {
  vector<Modint> fact(N + 2), finv(N + 2);
  fact[0] = finv[N + 1] = 1;
  for (int i = 1; i <= N + 1; i++) fact[i] = fact[i - 1] * i;
  finv[N + 1] /= fact[N + 1];
  for (int i = N; i >= 0; i--) finv[i] = finv[i + 1] * (i + 1);
  FormalPowerSeries<Modint> a((N + 1) / 2), b((N + 1) / 2);
  for (int i = 0; i <= (N - 1) / 2; i++) {
    a[i] = i & 1 ? -finv[i] * finv[N + 1 - i] : finv[i] * finv[N + 1 - i];
    b[i] = Modint(i + 1).pow(N);
  }
  FormalPowerSeries<Modint> ret = (a * b).part(N);
  for (int i = 0; i <= (N - 1) / 2; i++)
    ret[N - i - 1] = ret[i] = ret[i] * fact[N + 1];
  return ret;
}
