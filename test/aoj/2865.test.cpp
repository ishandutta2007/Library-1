#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/challenges/sources/JAG/Regional/2865"
#include <iostream>
#include "src/Optimization/PiecewiseLinearConvexfunction.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 long long d[N - 1], g[N];
 for (int i= 0; i < N - 1; ++i) cin >> d[i];
 for (int i= 0; i < N; ++i) cin >> g[i];
 PiecewiseLinearConvexfunction f;
 f.chinfty_right(), f.chinfty_left();
 for (int i= 0; i < N; ++i) {
  if (i) f.add_abs(d[i - 1], 0);
  f.add_linear(-g[i]);
  f.chmin_sliding_window(-1, 1);
  f.add_linear(g[i], g[i]);
 }
 cout << f(0) << '\n';
 return 0;
}