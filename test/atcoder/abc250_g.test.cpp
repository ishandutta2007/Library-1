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
 f.chinfty_right(), f.chinfty_left();
 for (int i= 0; i < N; ++i) {
  int P;
  cin >> P;
  f.add_linear(-P);
  f.chmin_sliding_window(-1, 1);
  f.add_linear(P);
  f.chinfty_left();
 }
 cout << -f.min() << '\n';
 return 0;
}