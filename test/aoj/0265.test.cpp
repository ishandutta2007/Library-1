// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Prelim/0265
// competitive-verifier: TLE 0.5
#include "src/Misc/compress.hpp"
#include "src/Geometry/Polygon.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 using R= long double;
 using u128= __uint128_t;
 for (int n; cin >> n && n;) {
  vector<Point<R>> ps(n);
  for (int i= 0; i < n; ++i) cin >> ps[i];
  Polygon g(ps);
  auto es= g.edges();
  vector<Segment<R>> ss;
  for (const auto &e: es) {
   vector<Point<R>> ps{e.p, e.q};
   for (const auto &f: es) {
    auto cp= cross_points(e, line_through(f.p, f.q));
    if (cp.size()) ps.emplace_back(cp[0]);
   }
   compress(ps);
   for (int i= ps.size(); --i;) ss.emplace_back(ps[i - 1], ps[i]);
  }
  int m= ss.size();
  u128 flg[n], all= (u128(1) << m) - 1;
  fill_n(flg, n, 0);
  for (int i= n; i--;)
   for (int j= m; j--;) flg[i]|= u128(g.in({ss[j].p, g[i]}) & g.in({ss[j].q, g[i]})) << j;
  int ans= n;
  for (int s= 1 << n; --s;) {
   int c= __builtin_popcount(s);
   if (ans <= c) continue;
   u128 f= 0;
   for (int i= n; i--;)
    if ((s >> i) & 1) f|= flg[i];
   if (f == all) ans= c;
  }
  cout << ans << '\n';
 }
 return 0;
}