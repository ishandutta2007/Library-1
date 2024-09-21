// competitive-verifier: PROBLEM https://atcoder.jp/contests/arc070/tasks/arc070_e
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// https://atcoder.jp/contests/arc070/tasks/arc070_c
#include <iostream>
#include "src/Optimization/PiecewiseLinearConvex.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 PiecewiseLinearConvex<long long> f;
 long long pw;
 for (int i= 0; i < N; ++i) {
  long long l, r;
  cin >> l >> r;
  long long w= r - l;
  if (i) f.chmin_slide_win(-w, pw);
  f.add_abs(1, l);
  pw= w;
 }
 cout << (long long)f.min().value() << '\n';
 return 0;
}