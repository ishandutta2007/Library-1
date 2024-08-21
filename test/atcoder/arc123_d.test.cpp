// competitive-verifier: PROBLEM https://atcoder.jp/contests/arc123/tasks/arc123_d
#include <iostream>
#include "src/Optimization/PiecewiseLinearConvex.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 PiecewiseLinearConvex<long long> f;
 int pA;
 for (int i= 0; i < N; ++i) {
  int A;
  cin >> A;
  if (i) f.shift(max(A - pA, 0)), f.chmin_cum();
  f.add_abs(1, 0), f.add_abs(1, A);
  pA= A;
 }
 cout << (long long)f.min().value() << '\n';
 return 0;
}