// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/0524
// competitive-verifier: TLE 0.5
#include <iostream>
#include <set>
#include <vector>
#include "src/Geometry/Point.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 for (int m; cin >> m && m;) {
  vector<Point<int>> ps(m);
  for (int i= 0; i < m; ++i) cin >> ps[i];
  int n;
  cin >> n;
  vector<Point<int>> qs(n);
  for (int i= 0; i < n; ++i) cin >> qs[i];
  set S(qs.begin(), qs.end());
  Point<int> ans;
  for (int i= n; i--;) {
   auto v= qs[i] - ps[0];
   bool isok= true;
   for (int j= m; --j;) isok&= S.count(ps[j] + v);
   if (isok) ans= v;
  }
  cout << ans.x << " " << ans.y << '\n';
 }
 return 0;
}