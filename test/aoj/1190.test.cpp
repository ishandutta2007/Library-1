// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/1190
// competitive-verifier: ERROR 0.000001
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "src/Geometry/Circle.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 cout << fixed << setprecision(12);
 using R= long double;
 for (int N; cin >> N && N;) {
  vector<Circle<R>> cs(N);
  for (int i= 0; i < N; i++) cin >> cs[i].o >> cs[i].r;
  vector<Point<R>> ps;
  for (int i= N; i--;) {
   ps.push_back(cs[i].o);
   for (int j= i; j--;) {
    auto cp= cross_points(cs[i], cs[j]);
    if (cp.size() <= 1) continue;
    ps.push_back((cp[0] + cp[1]) / 2);
    Segment s= {cp[0], cp[1]};
    for (int k= j; k--;) {
     cp= cross_points(cs[i], cs[k]);
     if (cp.size() <= 1) continue;
     cp= cross_points(s, Segment(cp[0], cp[1]));
     if (!cp.size()) continue;
     ps.push_back(cp[0]);
    }
   }
  }
  R ans= 0;
  for (const auto &p: ps) {
   R a= 1e10;
   for (const auto &c: cs) a= min(a, c.r * c.r - dist2(c.o, p));
   ans= max(ans, a);
  }
  cout << sqrt(ans) << '\n';
 }
 return 0;
}