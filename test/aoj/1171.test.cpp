// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/1171
// competitive-verifier: ERROR 0.00001
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include "src/Geometry/Segment.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using namespace geo;
 cout << fixed << setprecision(12);
 using R= long double;
 static constexpr R INF= 1e10;
 for (int n; cin >> n && n;) {
  vector<Segment<R>> ss(n);
  for (int i= 0; i < n; ++i) cin >> ss[i].p >> ss[i].q;
  Point<R> T, L;
  cin >> T >> L;
  auto score= [&](const vector<int> &sq) -> R {
   auto tar= T;
   auto ts= ss;
   for (int i: sq) {
    auto ref= reflect(line_through(ts[i].p, ts[i].q));
    for (auto &t: ts) t= ref(t);
    tar= ref(tar);
   }
   Segment beam(L, tar);
   ts= ss;
   auto isok= [&](const Segment<R> &range) {
    if (dot(beam.q - beam.p, range.q - range.p) < 0) return false;
    for (int j= n; j--;)
     if (intersection(range, ts[j]) == CROSSING) return false;
    return true;
   };
   Point prev= L;
   for (int i: sq) {
    auto cp= cross_points(beam, ts[i]);
    if (!cp.size()) return INF;
    if (!isok(Segment(prev, cp[0]))) return INF;
    prev= cp[0];
    auto ref= reflect(line_through(ts[i].p, ts[i].q));
    for (auto &t: ts) t= ref(t);
   }
   if (!isok(Segment(prev, tar))) return INF;
   return beam.length();
  };
  R ans= INF;
  for (int m= 0; m < 6; ++m) {
   int s= 1;
   for (int i= m; i--;) s*= n;
   for (; s--;) {
    vector<int> sq;
    for (int i= m, t= s; i--;) sq.push_back(t % n), t/= n;
    bool isok= true;
    for (int i= 0; i + 1 < m; ++i) isok&= sq[i] != sq[i + 1];
    if (!isok) continue;
    ans= min(ans, score(sq));
   }
  }
  cout << ans << '\n';
 }
 return 0;
}