#define PROBLEM "https://judge.yosupo.jp/problem/multipoint_evaluation"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Math/FormalPowerSeries.hpp"
#include "src/Math/ModInt.hpp"
#include "src/Math/SubproductTree.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<998244353>;
  using FPS = FormalPowerSeries<Mint>;
  int N, M;
  cin >> N >> M;
  FPS f(N);
  vector<Mint> x(M);
  for (int i = 0; i < N; i++) cin >> f[i];
  for (int j = 0; j < M; j++) cin >> x[j];
  vector<Mint> y = SubproductTree<Mint>(x).multi_eval(f);
  for (int j = 0; j < M; j++) cout << (j ? " " : "") << y[j];
  cout << endl;
  return 0;
}