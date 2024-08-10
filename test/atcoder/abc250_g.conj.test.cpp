// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc250/tasks/abc250_g
// competitive-verifier: TLE 1
#include <iostream>
#include "src/Optimization/PiecewiseLinearConvex.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 PiecewiseLinearConvex<int> f;
 for (int i= 0; i < N; ++i) {
  int P;
  cin >> P;
  f.add_abs(1, P);
  f.chmin_cum(true);
 }
 cout << f(0) << '\n';
 return 0;
}