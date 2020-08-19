#define PROBLEM "https://judge.yosupo.jp/problem/polynomial_taylor_shift"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Math/FormalPowerSeries.hpp"
#include "src/Math/ModInt.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<998244353>;
  using FPS = FormalPowerSeries<Mint>;
  int N, c;
  cin >> N >> c;
  FPS a(N);
  for (int i = 0; i < N; i++) cin >> a[i];
  auto b = a.shift(c);
  for (int i = 0; i < N; i++) cout << (i ? " " : "") << b[i];
  cout << endl;
  return 0;
}