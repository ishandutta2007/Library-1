#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2514"
#define ERROR "0.00000001"
#include <iostream>
#include <iomanip>
#include <vector>
#include "src/Misc/compress.hpp"
#include "src/Geometry/Polygon.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 cout << fixed << setprecision(12);
 using R= long double;
 const R INF= 1e10;
 for (int N; cin >> N && N;) {
  vector<Polygon<R>> gs;
  for (int i= 0; i < N; i++) {
   int m;
   cin >> m;
   vector<Point<R>> ps(m);
   for (int j= 0; j < m; j++) cin >> ps[j];
   gs.emplace_back(ps);
  }
  Point<R> S, G;
  cin >> S >> G;
  Point<R> lp, lq;
  cin >> lp >> lq;
  auto ref= reflect(line_through(lp, lq));
  Point refS= ref(S), refG= ref(G);
  int a= -1, b= -1, c= -1, d= -1;
  for (int i= 0; i < N; i++) {
   if (gs[i].where(S) != -1) a= i;
   if (gs[i].where(G) != -1) b= i;
   if (gs[i].where(refS) != -1) c= i;
   if (gs[i].where(refG) != -1) d= i;
  }
  if (a < 0 || b < 0 || c < 0 || d < 0 || a != b || c != d) {
   cout << "impossible" << '\n';
   continue;
  }
  Polygon h= ref(gs[c]);
  vector<Point<R>> ps{S, G};
  for (auto& p: gs[a])
   if (h.where(p) != -1) ps.push_back(p);
  for (auto& p: h)
   if (gs[a].where(p) != -1) ps.push_back(p);
  auto id= compress(ps);
  int M= ps.size();
  R dis[M][M];
  for (int i= M; i--;) fill_n(dis[i], M, INF), dis[i][i]= 0;
  for (int i= M; i--;)
   for (int j= i; j--;)
    if (Segment t{ps[i], ps[j]}; gs[a].in(t) && h.in(t)) dis[i][j]= dis[j][i]= t.length();
  for (int k= M; k--;)
   for (int i= M; i--;)
    for (int j= M; j--;) dis[i][j]= min(dis[i][j], dis[i][k] + dis[k][j]);
  auto ans= dis[id(S)][id(G)];
  if (ans == INF) cout << "impossible" << '\n';
  else cout << ans << '\n';
 }
 return 0;
}