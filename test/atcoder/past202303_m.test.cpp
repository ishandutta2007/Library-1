// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/past202303-open/tasks/past202303_m
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// max_right
#include <iostream>
#include <vector>
#include "src/DataStructure/SegmentTree.hpp"
using namespace std;
struct RmQ {
 using T= int;
 static T ti() { return -(1 << 30); }
 static T op(T a, T b) { return max(a, b); }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, M;
 cin >> N >> M;
 vector<int> A(N), B(M);
 for (int i= 0; i < N; ++i) cin >> A[i];
 for (int i= 0; i < M; ++i) cin >> B[i];
 SegmentTree<RmQ> seg(B);
 for (int i= 0; i < N; ++i) {
  int j= seg.max_right(0, [&](int x) { return x < A[i]; });
  if (j == M) return cout << "No\n" << i + 1 << '\n', 0;
  seg.set(j, seg[j] - A[i]);
 }
 cout << "Yes\n";
 for (int i= 0; i < M; ++i) cout << B[i] - seg[i] << " \n"[i + 1 == M];
 return 0;
}