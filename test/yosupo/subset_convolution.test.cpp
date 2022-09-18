#define PROBLEM "https://judge.yosupo.jp/problem/subset_convolution"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/SetPowerSeries.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = StaticModInt<998244353>;
  int N;
  cin >> N;
  vector<Mint> a(1 << N), b(1 << N);
  for (auto &ai : a) cin >> ai;
  for (auto &bi : b) cin >> bi;
  auto c = SetPowerSeries<20>::convolve(a, b);
  for (int i = 0; i < (1 << N); i++) cout << c[i] << " \n"[i + 1 == 1 << N];
  return 0;
}