/**
 * @title 複数の値代入と多項式補間
 * @category 数学
 *  どちらもO(N log^2 N)
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Math/ModInt.hpp"
#include "src/Math/FormalPowerSeries.hpp"
#undef call_from_test
#endif

template <typename Modint>
struct SubproductTree {
  using FPS = FormalPowerSeries<Modint>;
  int n;
  vector<Modint> xs;
  vector<FPS> buf;
  SubproductTree() {}
  SubproductTree(const vector<Modint> &_xs)
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
  vector<Modint> multi_eval(const FPS &f) {
    vector<Modint> res(n);
    function<void(FPS, int, int, int)> dfs = [&](FPS g, int l, int r, int k) {
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
  FPS interpolate(const vector<Modint> &ys) {
    FPS w = buf[1].diff();
    vector<Modint> vs = multi_eval(w);
    function<FPS(int, int, int)> dfs = [&](int l, int r, int k) {
      if (r - l == 1) return FPS({ys[l] / vs[l]});
      int m = (l + r) >> 1;
      return buf[k * 2 + 1] * dfs(l, m, k * 2)
             + buf[k * 2] * dfs(m, r, k * 2 + 1);
    };
    FPS res = dfs(0, n, 1);
    res.resize(n);
    return res;
  }
};
