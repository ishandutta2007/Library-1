#define PROBLEM "https://judge.yosupo.jp/problem/polynomial_interpolation"
#include <bits/stdc++.h>
#include "src/Old/ModInt.hpp"
#include "src/Old/FormalPowerSeries.hpp"
#include "src/Old/SubproductTree.hpp"
using namespace std;

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