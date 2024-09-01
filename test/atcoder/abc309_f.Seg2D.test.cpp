// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc309/tasks/abc309_f
// competitive-verifier: TLE 2.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <array>
#include "src/DataStructure/SegmentTree_2D.hpp"
using namespace std;
struct ROrQ {
 using T= bool;
 static T ti() { return 0; }
 static T op(T l, T r) { return l | r; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 vector<array<int, 3>> query;
 vector<array<int, 2>> xy;
 for (int i= 0; i < N; ++i) {
  int a[3];
  cin >> a[0] >> a[1] >> a[2];
  sort(a, a + 3);
  query.push_back({a[0], a[1], a[2]});
  xy.push_back({a[1], a[2]});
 }
 SegmentTree_2D<int, ROrQ> seg(xy);
 sort(query.begin(), query.end(), [](auto a, auto b) { return a[0] == b[0] ? a[1] > b[1] : a[0] < b[0]; });
 bool isok= false;
 for (auto [h, w, d]: query) {
  isok= seg.prod(0, w, 0, d);
  if (isok) break;
  seg.set(w, d, 1);
 }
 cout << (isok ? "Yes" : "No") << '\n';
 return 0;
}