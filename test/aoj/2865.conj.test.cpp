// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/JAG/Regional/2865
// competitive-verifier: TLE 1
// competitive-verifier: MLE 128
#include <iostream>
#include "src/Optimization/PiecewiseLinearConvex.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 long long d[N - 1], g[N];
 for (int i= 0; i < N - 1; ++i) cin >> d[i];
 for (int i= 0; i < N; ++i) cin >> g[i];
 PiecewiseLinearConvex<long long> f;
 for (int i= 0; i < N; ++i) {
  if (i) f.chmin_slide_win(-d[i - 1], d[i - 1]);
  f.add_abs(1, g[i]);
  f.add_const(-g[i]);
 }
 cout << (long long)-f.min().value() << '\n';
 return 0;
}