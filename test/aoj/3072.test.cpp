#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=3072"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "Math/FormalPowerSeries.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  FPS::init(998244353);
  int N, K, P;
  cin >> N >> K >> P;
  R p = FPS::mod_mul(P, FPS::inve[100]);
  R q = 1;
  FPS::mod_sub(q, p);
  FPS f(N + 1, p);
  f *= p;
  f *= FPS::mod_inverse(N);
  f = -f;
  f[0] = p;
  auto g = f.inv(K);
  R ans = 1;
  for (int i = 1; i < K; i++) {
    FPS::mod_sub(ans, FPS::mod_mul(g[i], q));
  }
  cout << ans << endl;
  return 0;
}
