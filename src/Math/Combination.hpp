#pragma once
#include <bits/stdc++.h>
/**
 * @title 二項係数など (階乗前計算) ($\mathbb{F}_p$)
 * @category 数学
 */

// BEGIN CUT HERE

template <class mint, std::size_t LIM = (1 << 24)>
class Combination {
  static inline mint _fact[LIM], _finv[LIM];
  static inline int lim = 0;
  static inline void set(int sz) {
    if (lim > sz) return;
    if (lim == 0) _fact[0] = 1, _finv[0] = 1, lim = 1;
    for (int i = lim; i <= sz; i++) _fact[i] = _fact[i - 1] * i;
    _finv[sz] = mint(1) / _fact[sz];
    for (int i = sz; i >= lim; i--) _finv[i - 1] = _finv[i] * i;
    lim = sz + 1;
  }

 public:
  static inline mint fact(int n) { return set(n), n < 0 ? mint(0) : _fact[n]; }
  static inline mint finv(int n) { return set(n), n < 0 ? mint(0) : _finv[n]; }
  static mint nPr(int n, int r) { return fact(n) * finv(n - r); }
  static mint nCr(int n, int r) { return nPr(n, r) * finv(r); }
  static mint nHr(int n, int r) { return !r ? mint(1) : nCr(n + r - 1, r); }
};