// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/1198
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include <algorithm>
#include "src/Geometry/Polygon.hpp"
#include "src/Geometry/Circle.hpp"
#include "src/Geometry/SegmentArrangement.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 using R= long double;
 for (int n, m; cin >> n >> m && n;) {
  vector<Circle<R>> cs(n);
  for (int i= 0; i < n; ++i) cin >> cs[i].o >> cs[i].r;
  vector<Segment<R>> ss;
  for (int i= n; i--;)
   for (int j= i; j--;)
    if (cross_points(cs[i], cs[j]).size()) ss.emplace_back(cs[i].o, cs[j].o);
  SegmentArrangement<R> sa(ss);
  int N= sa.face_size();
  vector<Polygon<R>> gs(N);
  for (int i= 0; i < N; ++i) {
   vector<Point<R>> ps;
   if (sa.is_outside(i)) continue;
   for (int s= sa.component_e(i), e= s;;) {
    ps.emplace_back(sa.point(sa.to_v(e)));
    e= sa.next(e);
    if (e == s) break;
   }
   gs[i]= Polygon(ps);
  }
  vector<string> ans;
  for (int k= 0; k < m; ++k) {
   Point<R> P, Q;
   cin >> P >> Q;
   bool isok= true;
   for (int i= n; i--;) isok&= cs[i].where(P) * cs[i].where(Q) >= 0;
   if (!isok) {
    ans.push_back("NO");
    continue;
   }
   isok= false;
   for (int i= n; i--;) isok|= cs[i].where(P) != -1 && cs[i].where(Q) != -1;
   if (isok) {
    ans.push_back("YES");
    continue;
   }
   isok= true;
   for (int i= N; i--;) {
    if (gs[i].size() <= 2) continue;
    isok&= gs[i].where(P) * gs[i].where(Q) >= 0;
   }
   ans.push_back(isok ? "YES" : "NO");
  }
  for (int k= 0; k < m; ++k) cout << ans[k] << " \n"[k == m - 1];
 }
 return 0;
}