#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/challenges/sources/JOI/Final/0661"
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
 f.add_inf(), f.add_inf(true);
 f.chmin_slide_win(-1, 1);
 f.add_linear(-a[0][1]);
 sum+= a[0][0] + a[0][1];
 for (int i= 1; i < N; ++i) {
  f.chmin_slide_win(-1, 1);
  f.add_linear(sum);
  f.chmin_slide_win(-1, 1);
  f.add_linear(-sum);
  f.shift(-1);
  f.add_inf(true);
  f.shift(2);
  f.add_inf();
  f.shift(-1);
  f.add_linear(-a[i][1]);
  sum+= a[i][0] + a[i][1];
 }
 ans-= f.min();
 cout << ans << '\n';
 return 0;
}