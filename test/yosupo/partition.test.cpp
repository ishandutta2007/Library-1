#define PROBLEM "https://judge.yosupo.jp/problem/partition_function"

#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/FormalPowerSeries.hpp"
#include "src/Math/fps_sequence.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<998244353>;
  int N;
  cin >> N;
  auto p = partition<Mint>(N);
  for (int i = 0; i <= N; i++) cout << (i ? " " : "") << p[i];
  cout << endl;
  return 0;
}