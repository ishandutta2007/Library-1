#define PROBLEM "https://yukicoder.me/problems/no/3046"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Math/ModInt.hpp"
#include "src/Math/FormalPowerSeries.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int K;
  cin >> K;
  int N;
  cin >> N;
  using Mint = ModInt<int(1e9 + 7)>;
  using FPS = FormalPowerSeries<Mint>;
  FPS f(1e5 + 10, 0);
  for (int i = 0; i < N; i++) {
    int x;
    cin >> x;
    f[x] = -1;
  }
  f[0] = 1;
  f.resize(K + 1);
  FPS ans = f.inv();
  cout << ans[K] << endl;
  return 0;
}