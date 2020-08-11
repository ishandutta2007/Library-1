#define PROBLEM "https://yukicoder.me/problems/444"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Math/FormalPowerSeries.hpp"
#include "src/Math/ModInt.hpp"
#include "src/Math/kitamasa.hpp"
#undef call_from_test

using Mint = ModInt<int(1e9 + 7)>;
Mint dp1[301][300 * 13 + 1];
Mint dp2[301][300 * 12 + 1];

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using FPS = FormalPowerSeries<Mint>;
  long long N;
  int P, C;
  cin >> N >> P >> C;
  int max_p = 13 * P + 1, max_c = 12 * C + 1;
  dp1[0][0] = 1;
  dp2[0][0] = 1;
  for (int v : {2, 3, 5, 7, 11, 13}) {
    for (int i = 0; i < P; i++) {
      for (int j = 0; j < max_p; j++) {
        if (j + v < max_p) dp1[i + 1][j + v] += dp1[i][j];
      }
    }
  }
  for (int v : {4, 6, 8, 9, 10, 12}) {
    for (int i = 0; i < C; i++) {
      for (int j = 0; j < max_c; j++) {
        if (j + v < max_c) dp2[i + 1][j + v] += dp2[i][j];
      }
    }
  }
  FPS p(max_p), c(max_c);
  for (int i = 0; i < max_p; i++) p[i] = dp1[P][i];
  for (int i = 0; i < max_c; i++) c[i] = dp2[C][i];
  FPS pc = p * c;
  vector<Mint> coefs(pc.size() - 1), a(pc.size() - 1, 1);
  for (int i = 0; i < pc.size() - 1; i++) {
    coefs[i] = pc[pc.size() - 1 - i];
  }
  cout << kitamasa(coefs, a, N + a.size() - 1) << endl;
  return 0;
}
