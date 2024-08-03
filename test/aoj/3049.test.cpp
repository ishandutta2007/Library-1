// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/3049
// competitive-verifier: ERROR 0.00001
#include <iostream>
#include <iomanip>
#include <vector>
#include "src/Geometry/Convex.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 using R= long double;
 int N;
 cin >> N;
 vector<Point<R>> qs(N);
 for (int i= 0; i < N; ++i) cin >> qs[i];
 Convex ch(qs);
 int M;
 cin >> M;
 vector<Point<R>> ps(M);
 for (int i= 0; i < M; i++) cin >> ps[i];
 R ans= 0;
 for (int i= 0; i < M; i++) {
  Convex dh= ch;
  for (int j= 0; j < M; j++)
   if (j != i) dh= dh.cut(bisector(ps[i], ps[j]));
  for (Point p: dh) ans= max(ans, dist(p, ps[i]));
 }
 cout << fixed << setprecision(12) << ans << '\n';
 return 0;
}