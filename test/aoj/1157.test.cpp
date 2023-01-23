#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1157"
#define ERROR "0.0001"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "src/Geometry/!geometry_temp.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geometry;
 cout << fixed << setprecision(12);
 for (int N; cin >> N && N;) {
  Segment s;
  cin >> s.p1 >> s.p2;
  Real ans= 1e10;
  for (int i= 0; i < N; i++) {
   Real l, d, r, u, h;
   cin >> l >> d >> r >> u >> h;
   Polygon g= {{l, d}, {r, d}, {r, u}, {l, u}};
   Real dis= dist(g, s);
   if (dis <= h) ans= min(ans, dis);
   else ans= min(ans, (dis * dis + h * h) / (2 * h));
  }
  cout << ans << endl;
 }
 return 0;
}