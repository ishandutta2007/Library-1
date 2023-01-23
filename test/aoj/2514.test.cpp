#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2514"
#define ERROR "0.00000001"
#include <iostream>
#include <iomanip>
#include <vector>
#include "src/Geometry/!geometry_temp.hpp"
#include "src/Geometry/circle_functions.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geometry;
 cout << fixed << setprecision(12);
 const Real INF= 1e10;
 for (int N; cin >> N && N;) {
  vector<Polygon> gs(N);
  for (int i= 0; i < N; i++) {
   int m;
   cin >> m;
   gs[i].resize(m);
   for (int j= 0; j < m; j++) cin >> gs[i][j];
  }
  Point S, G;
  cin >> S >> G;
  Line l;
  cin >> l.p1 >> l.p2;
  Point refS= l.reflect(S), refG= l.reflect(G);
  int a= -1, b= -1, c= -1, d= -1;
  for (int i= 0; i < N; i++) {
   if (gs[i].where(S) != OUT) a= i;
   if (gs[i].where(G) != OUT) b= i;
   if (gs[i].where(refS) != OUT) c= i;
   if (gs[i].where(refG) != OUT) d= i;
  }
  if (a < 0 || b < 0 || c < 0 || d < 0 || a != b || c != d) {
   cout << "impossible" << endl;
   continue;
  }
  Polygon h= l.reflect(gs[c]);
  vector<Point> ps= {S, G};
  for (Point p: gs[a])
   if (h.where(p) != OUT) ps.push_back(p);
  for (Point p: h)
   if (gs[a].where(p) != OUT) ps.push_back(p);
  int M= ps.size();
  Real dis[M][M];
  for (int i= 0; i < M; i++) fill(dis[i], dis[i] + M, INF), dis[i][i]= 0;
  for (int i= 0; i < M; i++)
   for (int j= i + 1; j < M; j++)
    if (gs[a].contains({ps[i], ps[j]}) && h.contains({ps[i], ps[j]})) dis[i][j]= dis[j][i]= dist(ps[i], ps[j]);
  for (int k= 0; k < M; k++)
   for (int i= 0; i < M; i++)
    for (int j= 0; j < M; j++) dis[i][j]= min(dis[i][j], dis[i][k] + dis[k][j]);
  if (dis[0][1] == INF) cout << "impossible" << endl;
  else cout << dis[0][1] << endl;
 }
 return 0;
}