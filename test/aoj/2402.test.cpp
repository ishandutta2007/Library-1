// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/2402
// competitive-verifier: ERROR 0.00000001
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
#include <iostream>
#include <iomanip>
#include <vector>
#include "src/Geometry/Polygon.hpp"
#include "src/Geometry/angle.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 cout << fixed << setprecision(12);
 using R= long double;
 for (int N, M, L; cin >> N >> M >> L && N;) {
  vector<Polygon<R>> stars(N);
  for (int i= 0; i < N; i++) {
   Point<R> p;
   R a, r;
   cin >> p >> a >> r;
   vector<Point<R>> ps(5);
   for (int j= 0; j < 5; j++) {
    R theta= degree_to_radian(a + j * 144 + 90);
    ps[j]= p + r * Point<R>{cos(theta), sin(theta)};
   }
   stars[i]= Polygon<R>(ps);
  }
  R dis[N][N];
  for (int i= N; i--;) dis[i][i]= 0;
  for (int i= N; i--;)
   for (int j= N; j--;) dis[i][j]= dis[j][i]= dist(stars[i], stars[j]);
  for (int k= N; k--;)
   for (int i= N; i--;)
    for (int j= N; j--;) dis[i][j]= min(dis[i][j], dis[i][k] + dis[k][j]);
  cout << dis[M - 1][L - 1] << '\n';
 }
 return 0;
}