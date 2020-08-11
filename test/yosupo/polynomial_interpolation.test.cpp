#define PROBLEM "https://judge.yosupo.jp/problem/polynomial_interpolation"

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
  int N;
  cin >> N;
  vector<Mint> x(N), y(N);
  for (int i = 0; i < N; i++) cin >> x[i];
  for (int i = 0; i < N; i++) cin >> y[i];
  FPS f = SubproductTree<Mint>(x).interpolate(y);
  for (int i = 0; i < N; i++) cout << (i ? " " : "") << f[i];
  cout << endl;
  return 0;
}