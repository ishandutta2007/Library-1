// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/1066
// competitive-verifier: ERROR 0.00000001
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include "src/Geometry/angle.hpp"
#include "src/Geometry/Circle.hpp"
#include "src/Geometry/Polygon.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 using Real= long double;
 cout << fixed << setprecision(12);
 for (int N, R, Q; cin >> N >> R >> Q && N;) {
  vector<Point<Real>> ps(N);
  for (int i= 0; i < N; ++i) cin >> ps[i];
  Polygon g(ps);
  int cur= -1;
  for (int i= N; i--;)
   if (!sgn(norm2(g[i]) - R * R)) cur= i;
  Circle<Real> c({0, 0}, R);
  while (Q--) {
   Real mn= M_PI * 2;
   int nxt= -1;
   for (int i= N; i--;) {
    if (cur == i) continue;
    Circle<Real> d(g[cur], dist(g[cur], g[i]));
    auto cp= cross_points(c, d);
    auto th= angle(g[i] - g[cur], cp[0] - g[cur]);
    if (mn > th) mn= th, nxt= i;
   }
   g= rotate(g[cur], mn)(g);
   cur= nxt;
   cout << g[cur].x << " " << g[cur].y << '\n';
  }
 }
 return 0;
}