#define PROBLEM "https://judge.yosupo.jp/problem/log_of_formal_power_series"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/FFT/fps_exp.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N;
  cin >> N;
  using Mint = StaticModInt<998244353>;
  vector<Mint> a(N);
  for (int i = 0; i < N; i++) cin >> a[i];
  auto b = log(a);
  for (int i = 0; i < N; i++) cout << b[i] << " \n"[i == N - 1];
  return 0;
}