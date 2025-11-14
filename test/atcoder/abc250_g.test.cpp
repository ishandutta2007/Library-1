// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc250/tasks/abc250_g
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Optimization/PiecewiseLinearConvex.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 PiecewiseLinearConvex<int> f;
 f.add_inf(), f.add_inf(true);
 for (int i= 0; i < N; ++i) {
  int P;
  cin >> P;
  f.add_linear(-P);
  f.chmin_slide_win(-1, 1);
  f.add_linear(P);
  f.add_inf();
 }
 cout << -f.min().value() << '\n';
 return 0;
}