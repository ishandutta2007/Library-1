#pragma once
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/FormalPowerSeries.hpp"
/**
 * @title 複数の値代入と多項式補間
 * @category 数学
 *  どちらもO(N log^2 N)
 */

// BEGIN CUT HERE

template <typename mint>
class SubproductTree {
  using poly = FormalPowerSeries<mint>;
  int n;
  std::vector<mint> xs;
  std::vector<poly> buf;
  void pre(int l, int r, int k) {
    if (r - l == 1) {
      buf[k] = {-xs[l], 1};
      return;
    }
    pre(l, (l + r) / 2, k * 2), pre((l + r) / 2, r, k * 2 + 1);
    buf[k] = buf[k * 2] * buf[k * 2 + 1];
  }

 public:
  SubproductTree() {}
  SubproductTree(const std::vector<mint> &_xs)
      : n(_xs.size()), xs(_xs), buf(4 * n) {
    pre(0, n, 1);
  }
  std::vector<mint> multi_eval(const poly &f) {
    std::vector<mint> ret(n);
    auto rec = [&](auto dfs, poly g, int l, int r, int k) -> void {
      g %= buf[k];
      if (r - l <= 128) {
        for (int i = l; i < r; i++) ret[i] = g.eval(xs[i]);
        return;
      }
      dfs(dfs, g, l, (l + r) / 2, k * 2);
      dfs(dfs, g, (l + r) / 2, r, k * 2 + 1);
    };
    return rec(rec, f, 0, n, 1), ret;
  }
  poly interpolate(const std::vector<mint> &ys) {
    poly w = buf[1].diff();
    std::vector<mint> vs = multi_eval(w);
    auto rec = [&](auto dfs, int l, int r, int k) -> poly {
      if (r - l == 1) return poly({ys[l] / vs[l]});
      return buf[k * 2 + 1] * dfs(dfs, l, (l + r) / 2, k * 2)
             + buf[k * 2] * dfs(dfs, (l + r) / 2, r, k * 2 + 1);
    };
    poly ret = rec(rec, 0, n, 1);
    return ret.retize(n), ret;
  }
};
