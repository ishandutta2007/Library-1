#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2009"
#include <iostream>
#include <iomanip>
#include <vector>
#include "src/Geometry/!geometry_temp.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geometry;
 cout << fixed << setprecision(12);
 for (int N; cin >> N && N;) {
  int ans= 1;
  vector<Segment> ss;
  for (int i= 0; i < N; i++) {
   Segment s;
   cin >> s.p1 >> s.p2;
   ans++;
   vector<Point> ps;
   for (Segment t: ss) {
    auto cp= cross_points(s, t);
    if (cp.size()) {
     bool isok= cp[0] != s.p1 && cp[0] != s.p2;
     for (Point p: ps) isok&= cp[0] != p;
     if (isok) {
      ans++;
      ps.push_back(cp[0]);
     }
    }
   }
   ss.push_back(s);
  }
  cout << ans << endl;
 }
 return 0;
}