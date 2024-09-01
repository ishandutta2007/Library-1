// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc217/tasks/abc217_h
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Optimization/PiecewiseLinearConvex.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 PiecewiseLinearConvex<long long> f;
 f.add_inf(), f.add_inf(true);
 long long pT= 0;
 for (int i= 0; i < N; ++i) {
  long long i64, D, X;
  cin >> i64 >> D >> X;
  long long dT= i64 - pT;
  f.chmin_slide_win(-dT, dT);
  if (D) f.add_ramp(1, X);
  else f.add_ramp(-1, X);
  pT= i64;
 }
 cout << (long long)f.min().value() << '\n';
 return 0;
}