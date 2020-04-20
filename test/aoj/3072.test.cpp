#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=3072"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "Math/FormalPowerSeries.hpp"
#include "Math/ModInt.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using Mint = ModInt<998244353>;
  using FPS = FormalPowerSeries<Mint>;
  int N, K, P;
  cin >> N >> K >> P;
  Mint p = Mint(P) / Mint(100);
  Mint q = Mint(1) - p;
  FPS f(N + 1, -p * p / Mint(N));
  f[0] = p;
  auto g = f.inv(K);
  Mint ans(1);
  for (int i = 1; i < K; i++) {
    ans -= g[i] * q;
  }
  cout << ans << endl;
  return 0;
}
