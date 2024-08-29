// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/3176
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include "src/Geometry/SegmentArrangement.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 int N, M;
 cin >> N >> M;
 vector<Point<int>> ps(N);
 for (int i= 0; i < N; ++i) cin >> ps[i];
 vector<Segment<int>> ss(M);
 for (int i= 0; i < M; ++i) {
  int u, v;
  cin >> u >> v;
  ss[i]= Segment(ps[--u], ps[--v]);
 }
 SegmentArrangement sa(ss);
 int ans= 0;
 for (int f= sa.face_size(); f--;) ans= max(ans, sa.area2(f));
 cout << ans << '\n';
 return 0;
}