#define PROBLEM "https://atcoder.jp/contests/abc230/tasks/abc230_h"

#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/FFT/FormalPowerSeries.hpp"
using namespace std;
signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using Mint = StaticModInt<998244353>;
  using FPS = FormalPowerSeries<Mint>;
  auto X = FPS::x();
  int W, K;
  cin >> W >> K;
  vector<Mint> g(W + 1, 0);
  for (int i = 0; i < K; i++) {
    int w;
    cin >> w;
    g[w] += 1;
  }
  FPS ans;
  ans.reset().set(X * (MSET(FPS(g) + ans) - 1));
  for (int i = 2; i <= W; i++) cout << ans[i] << '\n';
  return 0;
}