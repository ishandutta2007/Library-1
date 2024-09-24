// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc266/tasks/abc266_Ex
// competitive-verifier: TLE 1.5
// competitive-verifier: MLE 128
// https://atcoder.jp/contests/abc266/tasks/abc266_h
#include <iostream>
#include <vector>
#include "src/DataStructure/SegmentTree_2D.hpp"
using namespace std;
struct RMQ {
 using T= long long;
 static T ti() { return -(1ll << 60); }
 static T op(T l, T r) { return max(l, r); }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 vector<array<int, 4>> dat(N);
 for (int i= 0; i < N; ++i) cin >> dat[i][0] >> dat[i][1] >> dat[i][2] >> dat[i][3];
 sort(dat.begin(), dat.end(), [&](auto &l, auto &r) { return l[2] == r[2] ? l[0] < r[0] : l[2] < r[2]; });
 set<array<long long, 2>> st1, st2;
 st1.insert({0, 0}), st2.insert({0, 0});
 for (int i= 0; i <= N; ++i) {
  auto [t, x, y, a]= dat[i];
  st1.insert({x, x + y - t}), st2.insert({x, x - y + t});
 }
 long long ans= 0;
 SegmentTree_2D<long long, RMQ> seg1(st1), seg2(st2);
 static constexpr long long INF= 1ll << 60;
 seg1.set(0, 0, 0), seg2.set(0, 0, 0);
 for (int i= 0; i < N; ++i) {
  auto [t, x, y, a]= dat[i];
  auto dp= max(seg1.prod(-INF, x + 1, x + y - t, INF), seg2.prod(x, INF, -INF, x - y + t + 1));
  if (dp < 0) continue;
  dp+= a;
  ans= max(ans, dp);
  seg1.mul(x, x + y - t, dp), seg2.mul(x, x - y + t, dp);
 }
 cout << ans << '\n';
 return 0;
}