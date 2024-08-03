// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Final/0375
// competitive-verifier: ERROR 0.0000001
#include <iostream>
#include <iomanip>
#include <vector>
#include "src/Geometry/Polygon.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 using R= long double;
 cout << fixed << setprecision(10);
 Point<R> s, g;
 cin >> s >> g;
 int N;
 cin >> N;
 vector<Point<R>> ps(N);
 for (int i= 0; i < N; ++i) cin >> ps[i];
 Polygon<R> h(ps);
 for (const auto &e: h.edges()) {
  auto cp= cross_points(e, Segment(s, reflect(line_through(e.p, e.q))(g)));
  if (cp.size()) ps.push_back(cp[0]);
 }
 int M= ps.size();
 R d[M + 2][M + 2];
 for (int i= M + 2; i--;) fill_n(d[i], M + 2, 1e18), d[i][i]= 0;
 for (int i= M; i--;)
  for (int j= M; j--;)
   if (h.out({ps[i], ps[j]})) d[i][j]= d[j][i]= dist(ps[i], ps[j]);
 for (int i= M; i--;) {
  if (h.out({ps[i], s})) d[M][i]= dist(ps[i], s);
  if (h.out({ps[i], g})) d[i][M + 1]= dist(ps[i], g);
 }
 for (int k= M + 2; k--;)
  for (int i= M + 2; i--;)
   for (int j= M + 2; j--;) d[i][j]= min(d[i][j], d[i][k] + d[k][j]);
 cout << d[M][M + 1] << '\n';
 return 0;
}