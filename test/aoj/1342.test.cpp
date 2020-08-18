#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1342"

#define ERROR "0.000001"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Geometry/!geometry_temp.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using namespace geometry;
  cout << fixed << setprecision(12);
  for (int n, w; cin >> n >> w && n;) {
    Point ps[n];
    Real h[n];
    for (int i = 0; i < n; i++) cin >> ps[i] >> h[i];
    auto rad = [&](Real r, Real height) {
      if (r <= height) return r;
      return sqrt(height * (2 * r - height));
    };
    auto check = [&](Real r) {
      Real ma = rad(r, w);
      if (sgn(ma - 50) >= 0) return false;
      Polygon g
          = {{ma, ma}, {100 - ma, ma}, {100 - ma, 100 - ma}, {ma, 100 - ma}};
      vector<Circle> cs(n);
      for (int i = 0; i < n; i++) cs[i] = {ps[i], rad(r, h[i])};
      vector<Point> cps;
      for (int i = 0; i < 4; i++) {
        cps.push_back(g[i]);
        Segment s = {g[i], g[g.next(i)]};
        for (Circle c : cs) {
          auto cp = cross_points(s, c);
          for (Point p : cp) cps.push_back(p);
        }
      }
      for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) {
          auto cp = cross_points(cs[i], cs[j]);
          for (Point p : cp) cps.push_back(p);
        }
      for (Point p : cps) {
        bool isok = g.where(p) != OUT;
        for (Circle c : cs) isok &= c.where(p) != IN;
        if (isok) return true;
      }
      return false;
    };
    Real low = 0, high = 1e10;
    for (int cnt = 100; cnt--;) {
      Real mid = (low + high) / 2;
      if (check(mid))
        low = mid;
      else
        high = mid;
    }
    cout << low << endl;
  }
  return 0;
}