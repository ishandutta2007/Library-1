// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Final/0356
// competitive-verifier: ERROR 0.0000001
// competitive-verifier: TLE 0.5
#include <iostream>
#include <iomanip>
#include <vector>
#include "src/Geometry/Circle.hpp"
#include "src/Geometry/Polygon.hpp"
#include "src/Geometry/intersection_area.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 using Real= long double;
 cout << fixed << setprecision(10);
 int N, R;
 cin >> N >> R;
 vector<Polygon<Real>> gs(N);
 for (int i= 0; i < N; ++i) {
  Real x, w, h;
  cin >> x >> w >> h;
  gs[i]= Polygon<Real>({{x, 0}, {x + w, 0}, {x + w, h}, {x, h}});
 }
 gs.emplace_back(Polygon<Real>({{-200, 0}, {200, 0}, {200, -200}, {-200, -200}}));
 Real all= M_PI * R * R;
 Real low= 0, high= 200;
 for (int cnt= 100; cnt--;) {
  Real y= (low + high) / 2;
  Real area= 0;
  Circle<Real> c(Point<Real>(0, y), R);
  for (const auto &g: gs) area+= intersection_area(g, c);
  if (area * 2 < all) high= y;
  else low= y;
 }
 cout << low << '\n';
 return 0;
}