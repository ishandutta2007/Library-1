// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/3034
// competitive-verifier: ERROR 0.000001
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include "src/Geometry/Circle.hpp"
#include "src/Geometry/min_enclosing_circle.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using namespace geo;
 using R= long double;
 int N, M;
 cin >> N >> M;
 vector<Point<R>> ps(N);
 for (int i= 0; i < N; ++i) cin >> ps[i];
 int all= 1 << N;
 R min_r[all];
 for (int S= all; --S;) {
  vector<Point<R>> qs;
  for (int i= N; i--;)
   if ((S >> i) & 1) qs.push_back(ps[i]);
  min_r[S]= min_enclosing_circle(qs).r;
 }
 R dp[all];
 fill_n(dp, all, 1e10), dp[0]= 0;
 for (int j= M; j--;)
  for (int S= all; --S;)
   for (int T= S; T; --T&= S) dp[S]= min(dp[S], max(dp[S ^ T], min_r[T]));
 cout << fixed << setprecision(12) << dp[all - 1] << '\n';
 return 0;
}