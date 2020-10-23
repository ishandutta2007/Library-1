#pragma once
#include <bits/stdc++.h>
#include "src/Math/Modint.hpp"
#include "src/Math/FormalPowerSeries.hpp"
/**
 * @title 複数の値代入と多項式補間
 * @category 数学
 *  どちらもO(N log^2 N)
 */

// BEGIN CUT HERE

template <typename mint>
struct SubproductTree {
  using poly = FormalPowerSeries<mint>;
  int n;
  std::vector<mint> xs;
  std::vector<poly> buf;
  SubproductTree() {}
  SubproductTree(const std::vector<mint> &_xs)
      : n(_xs.size()), xs(_xs), buf(4 * n) {
    pre(0, n, 1);
  }
  void pre(int l, int r, int k) {
    if (r - l == 1) {
      buf[k] = {-xs[l], 1};
      return;
    }
    int m = (l + r) >> 1;
    pre(l, m, k * 2), pre(m, r, k * 2 + 1);
    buf[k] = buf[k * 2] * buf[k * 2 + 1];
  }
  std::vector<mint> multi_eval(const poly &f) {
    std::vector<mint> res(n);
    std::function<void(poly, int, int, int)> dfs
        = [&](poly g, int l, int r, int k) {
            g %= buf[k];
            if (r - l <= 128) {
              for (int i = l; i < r; i++) res[i] = g.eval(xs[i]);
              return;
            }
            int m = (l + r) >> 1;
            dfs(g, l, m, k * 2), dfs(g, m, r, k * 2 + 1);
          };
    dfs(f, 0, n, 1);
    return res;
  }
  poly interpolate(const std::vector<mint> &ys) {
    poly w = buf[1].diff();
    std::vector<mint> vs = multi_eval(w);
    std::function<poly(int, int, int)> dfs = [&](int l, int r, int k) {
      if (r - l == 1) return poly({ys[l] / vs[l]});
      int m = (l + r) >> 1;
      return buf[k * 2 + 1] * dfs(l, m, k * 2)
             + buf[k * 2] * dfs(m, r, k * 2 + 1);
    };
    poly res = dfs(0, n, 1);
    res.resize(n);
    return res;
  }
};
