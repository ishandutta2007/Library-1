#define PROBLEM \
  "https://judge.yosupo.jp/problem/composition_of_formal_power_series"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/FormalPowerSeries.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<998244353>;
  using FPS = FormalPowerSeries<Mint>;
  int N;
  cin >> N;
  FPS a(N), b(N);
  for (int i = 0; i < N; i++) cin >> a[i];
  for (int i = 0; i < N; i++) cin >> b[i];
  FPS c = a.comp(b);
  for (int i = 0; i < N; i++) cout << (i ? " " : "") << c[i];
  cout << '\n';
  return 0;
}