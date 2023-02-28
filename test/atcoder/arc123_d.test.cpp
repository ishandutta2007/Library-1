#define PROBLEM "https://atcoder.jp/contests/arc123/tasks/arc123_d"
#include <iostream>
#include "src/Optimization/PiecewiseLinearConvexfunction.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 PiecewiseLinearConvexfunction f;
 long long pA;
 for (int i= 0; i < N; ++i) {
  long long A;
  cin >> A;
  if (i) f.shift(max(A - pA, 0ll)), f.cumulative_chmin();
  f.add_abs(1, 0), f.add_abs(1, A);
  pA= A;
 }
 cout << f.min() << '\n';
 return 0;
}