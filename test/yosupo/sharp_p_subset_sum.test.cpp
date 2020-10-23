#define PROBLEM "https://judge.yosupo.jp/problem/sharp_p_subset_sum"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/FormalPowerSeries.hpp"
using namespace std;

// log(1+x^s_1)(1+x^s_2)...(1+x^s_N)=log(1+x^s_1)+log(1+x^s_2)+...log(1+x^s_N)
// log(1+x)=x-x^2/2+x^3/3-x^4/4...
signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<998244353>;
  using FPS = FormalPowerSeries<Mint>;
  int N, T;
  cin >> N >> T;
  int c[T + 1];
  fill(c, c + T + 1, 0);
  for (int i = 0; i < N; i++) {
    int s;
    cin >> s;
    c[s]++;
  }
  FPS a(T + 1);
  for (int t = 1; t <= T; t++)
    if (c[t]) {
      for (int j = 1; j * t <= T; j++) {
        Mint tmp = Mint(c[t]) / Mint(j);
        a[j * t] += j & 1 ? tmp : -tmp;
      }
    }
  FPS ans = a.exp();
  for (int t = 1; t <= T; t++) {
    cout << (t ? " " : "") << ans[t];
  }
  cout << endl;
  return 0;
}