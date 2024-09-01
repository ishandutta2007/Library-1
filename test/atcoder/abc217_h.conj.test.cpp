// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc217/tasks/abc217_h
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 128
#include <iostream>
#include "src/Optimization/PiecewiseLinearConvex.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 PiecewiseLinearConvex<long long> f;
 long long pT= 0;
 for (int i= 0; i < N; ++i) {
  long long i64, D, X;
  cin >> i64 >> D >> X;
  long long dT= i64 - pT;
  f.add_abs(dT, 0);
  f.add_linear(-X);
  if (D) f.chmin_slide_win(0, 1);
  else f.chmin_slide_win(-1, 0);
  f.add_linear(X);
  pT= i64;
 }
 cout << (long long)-f(0).value() << '\n';
 return 0;
}