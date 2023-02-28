#define PROBLEM "https://atcoder.jp/contests/abc217/tasks/abc217_h"
#include <iostream>
#include "src/Optimization/PiecewiseLinearConvexfunction.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 PiecewiseLinearConvexfunction f;
 f.chinfty_right(), f.chinfty_left();
 long long pT= 0;
 for (int i= 0; i < N; ++i) {
  long long i64, D, X;
  cin >> i64 >> D >> X;
  long long dT= i64 - pT;
  f.chmin_sliding_window(-dT, dT);
  if (D) f.add_ax_bx_c(0, 1, X);
  else f.add_ax_bx_c(-1, 0, X);
  pT= i64;
 }
 cout << f.min() << '\n';
 return 0;
}