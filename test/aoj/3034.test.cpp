#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=3034"
#define ERROR "0.000001"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include "src/Geometry/!geometry_temp.hpp"
#include "src/Geometry/circle_functions.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using namespace geometry;
 int N, M;
 cin >> N >> M;
 vector<Point> ps(N);
 for (int i= 0; i < N; i++) cin >> ps[i];
 Real dp[1 << N][M + 1];
 Real min_r[1 << N];
 for (int S= 1; S < (1 << N); S++) {
  fill(dp[S], dp[S] + M + 1, 1e10);
  vector<Point> qs;
  for (int i= 0; i < N; i++)
   if ((S >> i) & 1) qs.push_back(ps[i]);
  min_r[S]= min_enclosing_circle(qs).r;
 }
 fill(dp[0], dp[0] + M + 1, 0);
 for (int S= 1; S < (1 << N); S++) {
  for (int j= 1; j <= M; j++) {
   for (int T= S; T; T= (T - 1) & S) {
    dp[S][j]= min(dp[S][j], max(dp[S ^ T][j - 1], min_r[T]));
   }
  }
 }
 cout << fixed << setprecision(12) << dp[(1 << N) - 1][M] << endl;
 return 0;
}