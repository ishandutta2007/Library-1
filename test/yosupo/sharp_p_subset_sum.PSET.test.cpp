#define PROBLEM "https://judge.yosupo.jp/problem/sharp_p_subset_sum"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/FFT/FormalPowerSeries.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 using FPS= FormalPowerSeries<Mint>;
 int N, T;
 cin >> N >> T;
 vector<Mint> c(T + 1, 0);
 for (int i= 0; i < N; i++) {
  int s;
  cin >> s, c[s]+= 1;
 }
 auto ans= PSET(FPS(c));
 for (int t= 1; t <= T; t++) cout << ans[t] << " \n"[t == T];
 return 0;
}