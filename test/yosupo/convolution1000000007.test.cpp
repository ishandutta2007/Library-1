#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod_1000000007"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "Math/FormalPowerSeries.hpp"
#include "Math/ModInt.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, M;
  cin >> N >> M;
  using Mint = ModInt<int(1e9 + 7)>;
  using FPS = FormalPowerSeries<Mint>;
  FPS a(N), b(M);
  for (int i = 0; i < N; i++) cin >> a[i];
  for (int j = 0; j < M; j++) cin >> b[j];
  auto c = a * b;
  for (int k = 0; k < N + M - 1; k++) {
    cout << c[k] << (k < N + M - 2 ? " " : "\n");
  }
  cout << flush;
  return 0;
}