// competitive-verifier: PROBLEM https://www2.ioi-jp.org/camp/2019/2019-sp-tasks/day1/examination.pdf
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <set>
#include "src/DataStructure/SegmentTree_2D.hpp"
using namespace std;
struct RSQ {
 using T= int;
 static T ti() { return 0; }
 static T op(T l, T r) { return l + r; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, Q;
 cin >> N >> Q;
 vector<array<int, 4>> query;
 set<array<int, 2>> st;
 for (int i= 0; i < N; ++i) {
  int S, T;
  cin >> S >> T;
  st.insert({S, T});
  query.push_back({-(S + T), -1, S, T});
 }
 for (int i= 0; i < Q; ++i) {
  int X, Y, Z;
  cin >> X >> Y >> Z;
  query.push_back({-Z, i, X, Y});
 }
 sort(query.begin(), query.end());
 vector<int> ans(Q);
 SegmentTree_2D<int, RSQ> seg(st);
 static constexpr int INF= 0x7fffffff;
 for (auto [z, i, x, y]: query) {
  if (i < 0) seg.mul(x, y, 1);
  else ans[i]= seg.prod(x, INF, y, INF);
 }
 for (int i= 0; i < Q; ++i) cout << ans[i] << '\n';
 return 0;
}