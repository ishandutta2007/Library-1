#pragma once
#include <bits/stdc++.h>
/**
 * @title 二項係数など
 * @category 数学
 */

// BEGIN CUT HERE

template <class mint>
struct Combination {
 private:
  static inline std::vector<mint> fc = {1}, iv = {1};
  static inline void set(int sz) {
    int lim = fc.size(), i;
    if (lim > sz) return;
    for (fc.resize(sz + 1), iv.resize(sz + 1), i = lim; i <= sz; i++)
      fc[i] = fc[i - 1] * i;
    for (iv[sz] = mint(1) / fc[sz], i = sz; i >= lim; i--)
      iv[i - 1] = iv[i] * i;
  }

 public:
  static inline mint fact(int n) { return set(n), n < 0 ? mint(0) : fc[n]; }
  static inline mint finv(int n) { return set(n), n < 0 ? mint(0) : iv[n]; }
  static mint nPr(int n, int r) { return fact(n) * finv(n - r); }
  static mint nCr(int n, int r) { return nPr(n, r) * finv(r); }
  static mint nHr(int n, int r) { return !r ? mint(1) : nCr(n + r - 1, r); }
};