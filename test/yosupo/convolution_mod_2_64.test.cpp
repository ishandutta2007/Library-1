#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod_2_64"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/FormalPowerSeries.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using FPS = FormalPowerSeries<unsigned long long>;
  int N, M;
  cin >> N >> M;
  FPS a(N), b(M);
  for (int i = 0; i < N; i++) cin >> a[i];
  for (int j = 0; j < M; j++) cin >> b[j];
  auto c = a * b;
  c.resize(N + M - 1);
  for (int k = 0; k < N + M - 1; k++) {
    cout << c[k] << (k < N + M - 2 ? " " : "\n");
  }
  cout << flush;
  return 0;
}