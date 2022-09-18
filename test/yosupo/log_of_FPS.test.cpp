#define PROBLEM "https://judge.yosupo.jp/problem/log_of_formal_power_series"
#include <bits/stdc++.h>
#include "src/Old/ModInt.hpp"
#include "src/Old/FormalPowerSeries.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N;
  cin >> N;
  using Mint = ModInt<998244353>;
  using FPS = FormalPowerSeries<Mint>;
  FPS a(N);
  for (int i = 0; i < N; i++) cin >> a[i];
  auto b = a.log();
  for (int i = 0; i < N; i++) cout << (i ? " " : "") << b[i];
  cout << '\n';
  return 0;
}