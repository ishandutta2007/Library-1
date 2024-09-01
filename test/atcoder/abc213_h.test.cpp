// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc213/tasks/abc213_h
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/FFT/FormalPowerSeries.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 using FPS= FormalPowerSeries<Mint, 40'010>;
 int N, M, T;
 cin >> N >> M >> T;
 std::vector<Mint> p[M];
 int a[M], b[M];
 for (int i= 0; i < M; i++) {
  cin >> a[i] >> b[i], a[i]--, b[i]--;
  p[i].resize(T, 0);
  for (int j= 0; j < T; j++) cin >> p[i][j];
 }
 auto X= FPS::x();
 FPS d[N];
 FPS::Resetter r[N];
 for (int s= 0; s < N; s++) r[s]= d[s].reset();
 for (int s= 0; s < N; s++) {
  FPS tmp;
  for (int i= 0; i < M; i++) {
   if (b[i] == s) tmp= d[a[i]] * p[i] + tmp;
   if (a[i] == s) tmp= d[b[i]] * p[i] + tmp;
  }
  tmp= tmp * X;
  if (!s) tmp= tmp + 1;
  r[s].set(tmp);
 }
 cout << d[0][T] << '\n';
 return 0;
}