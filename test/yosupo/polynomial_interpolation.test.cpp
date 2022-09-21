#define PROBLEM "https://judge.yosupo.jp/problem/polynomial_interpolation"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/FFT/SubProductTree.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = StaticModInt<998244353>;
  int N;
  cin >> N;
  std::vector<Mint> x(N), y(N);
  for (int i = 0; i < N; i++) cin >> x[i];
  for (int i = 0; i < N; i++) cin >> y[i];
  auto ans = SubProductTree(x).interpolate(y);
  for (int i = 0; i < N; i++) cout << ans[i] << " \n"[i == N - 1];
  return 0;
}