#define PROBLEM "https://judge.yosupo.jp/problem/bernoulli_number"
#include <bits/stdc++.h>
#include "src/Old/ModInt.hpp"
#include "src/Old/FormalPowerSeries.hpp"
#include "src/Old/fps_sequence.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<998244353>;
  int N;
  cin >> N;
  auto B = bernoulli<Mint>(N);
  for (int i = 0; i <= N; i++) cout << (i ? " " : "") << B[i];
  cout << endl;
  return 0;
}