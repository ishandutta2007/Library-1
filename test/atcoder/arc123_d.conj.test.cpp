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
 f.add_inf(), f.add_inf(true);
 int pA;
 for (int i= 0; i < N; ++i) {
  int A;
  cin >> A;
  if (i) f.add_linear(max(A - pA, 0)), f.add_inf(true);
  f.chmin_slide_win(-1, 1);
  f.add_linear(-A);
  f.chmin_slide_win(-1, 1);
  f.add_linear(A);
  pA= A;
 }
 cout << (long long)-f(0).value() << '\n';
 return 0;
}