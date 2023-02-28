#define PROBLEM "https://atcoder.jp/contests/abc250/tasks/abc250_g"
#include <iostream>
#include "src/Optimization/PiecewiseLinearConvexfunction.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 PiecewiseLinearConvexfunction f;
 for (int i= 0; i < N; ++i) {
  int P;
  cin >> P;
  f.add_ax_bx_c(-1, 1, P);
  f.cumulative_chmin_rev();
 }
 cout << f(0) << '\n';
 return 0;
}