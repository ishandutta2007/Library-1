#pragma once
#include <bits/stdc++.h>
#include "src/Old/ModInt.hpp"
#include "src/Old/FormalPowerSeries.hpp"
/**
 * @title 線形漸化式の高速計算
 * @category 数学
 *  O(NlogNlogk)
 */

// BEGIN CUT HERE

// b[0] = a[0], b[1] = a[1], ..., b[N-1] = a[N-1]
// b[n] = c[0] * b[n-N] + c[1] * b[n-N+1] + ... + c[N-1] * b[n-1] (n >= N)
// return b[k]

template <class mint>
mint kitamasa(const std::vector<mint> &c, const std::vector<mint> &a,
              std::uint64_t k) {
  assert(a.size() == c.size());
  int N = a.size();
  if (k < (std::uint64_t)N) return a[k];
  std::uint64_t mask = (std::uint64_t(1) << (63 - __builtin_clzll(k))) >> 1;
  FormalPowerSeries<mint> f(N + 1), r({1, 0});
  f[0] = 1;
  for (int i = 0; i < N; i++) f[N - i] = -c[i];
  if (N < 1024) {  // naive
    r = r.quorem_rev_n(f).second;
    for (; mask; mask >>= 1) {
      r = r.mul(r);
      if (k & mask) r.push_back(0);
      r = r.quorem_rev_n(f).second;
    }
  } else {
    FormalPowerSeries<mint> inv = f.inv();
    r = r.quorem_rev_con(f, inv).second;
    for (; mask; mask >>= 1) {
      r = r.mul(r);
      if (k & mask) r.push_back(0);
      r = r.quorem_rev_con(f, inv).second;
    }
  }
  mint ret(0);
  for (int i = 0; i < N; i++) ret += r[N - i - 1] * a[i];
  return ret;
}