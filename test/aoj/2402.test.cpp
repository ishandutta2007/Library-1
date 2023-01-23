#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2402"
#define ERROR "0.00000001"
#include <iostream>
#include <iomanip>
#include <vector>
#include "src/Geometry/!geometry_temp.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geometry;
 for (int N, M, L; cin >> N >> M >> L && N;) {
  vector<vector<Segment>> stars(N);
  for (int i= 0; i < N; i++) {
   Point p;
   Real a, r;
   cin >> p >> a >> r;
   Point ps[5];
   for (int j= 0; j < 5; j++) {
    Real theta= degree_to_radian(a + j * 72 + 90);
    ps[j]= p + r * Point({cos(theta), sin(theta)});
   }
   for (int j= 0; j < 5; j++) {
    int k= (j + 2) % 5;
    stars[i].push_back({ps[j], ps[k]});
   }
  }
  Real dis[N][N];
  for (int i= 0; i < N; i++) {
   dis[i][i]= 0;
   for (int j= i + 1; j < N; j++) {
    dis[i][j]= 1e9;
    for (Segment s: stars[i])
     for (Segment t: stars[j]) dis[i][j]= min(dis[i][j], dist(s, t));
    dis[j][i]= dis[i][j];
   }
  }
  for (int k= 0; k < N; k++)
   for (int i= 0; i < N; i++)
    for (int j= 0; j < N; j++) dis[i][j]= min(dis[i][j], dis[i][k] + dis[k][j]);
  cout << fixed << setprecision(12) << dis[M - 1][L - 1] << endl;
 }
 return 0;
}