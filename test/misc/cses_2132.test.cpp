// competitive-verifier: PROBLEM https://cses.fi/problemset/task/2132/
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

#include <sstream>
#include <string>
#include <cassert>
#include "src/Optimization/PiecewiseLinearConvex.hpp"
using namespace std;
signed main() {
 int n;
 cin >> n;
 PiecewiseLinearConvex<int> f;
 for (int i= 0; i < n; ++i) {
  int x;
  cin >> x;
  f.chmin_cum();
  f.add_abs(1, x);
 }
 cout << f.min().value() << '\n';
 return 0;
}