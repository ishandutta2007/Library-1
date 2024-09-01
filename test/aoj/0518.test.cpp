// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/0518
// competitive-verifier: TLE 2.5
// competitive-verifier: MLE 64
#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include "src/Geometry/Point.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 for (int n; cin >> n && n;) {
  vector<Point<int>> ps(n);
  for (int i= 0; i < n; ++i) cin >> ps[i];
  set S(ps.begin(), ps.end());
  int ans= 0;
  for (int i= n; i--;)
   for (int j= i; j--;) {
    auto p1= ps[i], p2= ps[j], v= !(p2 - p1);
    if (S.count(p1 + v) && S.count(p2 + v)) ans= max(ans, dist2(p1, p2));
    if (S.count(p1 - v) && S.count(p2 - v)) ans= max(ans, dist2(p1, p2));
   }
  cout << ans << '\n';
 }
 return 0;
}