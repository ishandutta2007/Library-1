#define PROBLEM "https://yukicoder.me/problems/no/1145"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Math/FormalPowerSeries.hpp"
#include "src/Math/ModInt.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<998244353>;
  using FPS = FormalPowerSeries<Mint>;
  int N, M;
  cin >> N >> M;
  int sz = 1 << (32 - __builtin_clz(N));
  vector<FPS> buf(2 * sz + 1, {1});
  for (int i = 0; i < N; i++) {
    Mint A;
    cin >> A;
    buf[sz + i] = {1, -A};
  }
  for (int k = sz - 1; k >= 0; k--)
    buf[k] = (buf[2 * k + 1] * buf[2 * k + 2]).pre(M + 1);
  auto ans = -buf[0].log(M + 1);
  for (int i = 1; i <= M; i++) cout << (i - 1 ? " " : "") << ans[i] * i;
  cout << endl;
  return 0;
}