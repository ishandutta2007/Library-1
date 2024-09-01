// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/JOI/Final/0661
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Optimization/PiecewiseLinearConvex.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 long long ans= 0;
 int a[N][2];
 for (int i= 0; i < N; ++i) a[i][0]= a[i][1]= -1;
 for (int i= 0; i < N + N; ++i) {
  int X, Y;
  cin >> X >> Y;
  if (X < 1) {
   ans+= 1 - X;
   if (Y < 1) ++a[0][0], ans+= 1 - Y;
   else if (Y > 2) ++a[0][1], ans+= Y - 2;
   else ++a[0][Y - 1];
  } else if (X > N) {
   ans+= X - N;
   if (Y < 1) ++a[N - 1][0], ans+= 1 - Y;
   else if (Y > 2) ++a[N - 1][1], ans+= Y - 2;
   else ++a[N - 1][Y - 1];
  } else {
   if (Y < 1) ++a[X - 1][0], ans+= 1 - Y;
   else if (Y > 2) ++a[X - 1][1], ans+= Y - 2;
   else ++a[X - 1][Y - 1];
  }
 }
 int sum= 0;
 PiecewiseLinearConvex<int> f;
 for (int i= 0; i < N; ++i) {
  f.add_abs(1, 0);
  f.add_abs(1, -sum);
  f.add_linear(-1);
  f.chmin_cum();
  f.add_linear(2);
  f.chmin_cum(true);
  f.add_linear(-1);
  f.shift(-a[i][1]);
  sum+= a[i][0] + a[i][1];
 }
 ans+= f(0).value();
 cout << ans << '\n';
 return 0;
}