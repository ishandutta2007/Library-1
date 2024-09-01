// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/0273
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <queue>
#include <vector>
#include "src/Geometry/SegmentArrangement.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 using R= long double;
 for (int C, W; cin >> C >> W && C;) {
  vector<Point<R>> ps(C);
  for (int i= 0; i < C; ++i) cin >> ps[i];
  vector<Segment<R>> ss(W);
  for (int i= 0; i < W; ++i) {
   int s, t;
   cin >> s >> t;
   ss[i]= {ps[--s], ps[--t]};
  }
  SegmentArrangement sa(ss);
  int n= sa.face_size();
  int s= -1;
  for (int f= n; f--;)
   if (sa.is_outside(f)) s= f;
  vector<int> used(n);
  queue<pair<int, int>> que;
  que.emplace(0, s), used[s]= true;
  int ans= 0;
  while (!que.empty()) {
   auto [d, f]= que.front();
   que.pop();
   for (int a= sa.component_e(f), e= a;;) {
    int g= sa.incident_face(sa.twin(e));
    if (!used[g]) que.emplace(d + 1, g), ans= max(ans, d + 1);
    used[g]= true;
    if (e= sa.next(e); e == a) break;
   }
  }
  cout << ans << '\n';
 }
 return 0;
}