// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/3056
// competitive-verifier: ERROR 0.000001
// competitive-verifier: TLE 1
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include "src/Geometry/Point.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 using R= long double;
 const R sq3= sqrt(3.0);
 int N;
 cin >> N;
 vector<Point<R>> ps(N);
 for (int i= 0; i < N; i++) cin >> ps[i];
 R ans= 1e12;
 for (int i= N; i--;) {
  auto p= ps[i], q= ps[(i + 1) % N], v= q - p;
  R mx= 0, mn= 0;
  for (const auto &r: ps) {
   auto w= r - p;
   R y= cross(v, w), x= dot(v, w), yy= y / sq3;
   mx= max(mx, x + yy), mn= min(mn, x - yy);
  }
  ans= min(ans, (mx - mn) / norm(v));
 }
 cout << fixed << setprecision(12) << ans << '\n';
 return 0;
}