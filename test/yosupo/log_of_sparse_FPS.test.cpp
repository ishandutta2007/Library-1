#define PROBLEM \
  "https://judge.yosupo.jp/problem/log_of_formal_power_series_sparse"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/sparse_fps.hpp"
using namespace std;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using Mint = StaticModInt<998244353>;
  long long N, K;
  cin >> N >> K;
  vector<Mint> a(N);
  while (K--) {
    int i;
    cin >> i;
    cin >> a[i];
  }
  auto ans = sparse_log(a, N);
  for (int i = 0; i < N; i++) cout << ans[i] << " \n"[i == N - 1];
  return 0;
}