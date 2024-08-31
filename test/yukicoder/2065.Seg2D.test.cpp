// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/2065
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include <array>

// kdt, sqrtdc だと TLE

#include "src/DataStructure/SegmentTree_2D.hpp"
using namespace std;
struct RSQ {
 using T= pair<int, long long>;
 static T ti() { return {0, 0}; }
 static T op(const T &l, const T &r) { return {l.first + r.first, l.second + r.second}; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 vector<tuple<int, int, pair<int, long long>>> xyv(N);
 for (int i= 0; i < N; ++i) {
  int A;
  cin >> A;
  xyv[i]= {i, A, {1, A}};
 }
 SegmentTree_2D<int, RSQ> seg(xyv);
 while (Q--) {
  int L, R, X;
  cin >> L >> R >> X, --L;
  auto [cnt, sum]= seg.prod(L, R, 0, X);
  cout << sum + (long long)(R - L - cnt) * X << '\n';
 }
 return 0;
}