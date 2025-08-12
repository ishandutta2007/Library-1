// competitive-verifier: PROBLEM https://cses.fi/problemset/task/2132/
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

#include <iostream>
#include "src/Optimization/PiecewiseLinearConvex.hpp"
using namespace std;
signed main() {
 int n;
 cin >> n;
 PiecewiseLinearConvex<int> f;
 f.add_inf();
 for (int i= 0; i < n; ++i) {
  int x;
  cin >> x;
  f.add_inf(true);
  f.add_linear(-x);
  f.chmin_slide_win(-1, 1);
  f.add_linear(x);
 }
 cout << -f(0).value() << '\n';
 return 0;
}