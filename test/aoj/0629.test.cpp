// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/JOI/Final/0629
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// max_right
#include <iostream>
#include <vector>
#include "src/DataStructure/SegmentTree.hpp"
using namespace std;
struct M {
 using T= long long;
 using E= long long;
 static T ti() { return -(1ll << 60); }
 static T op(T l, T r) { return max(l, r); }
 static void mp(T &v, E y) { v+= y; }
 static void cp(E &x, E y) { x+= y; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, Q;
 cin >> N >> Q;
 int X[Q], D[Q], L[Q];
 for (int q= 0; q < Q; ++q) cin >> X[q] >> D[q] >> L[q];
 SegmentTree<M> seg1(N + 1, [&](int i) { return i; }), seg2(N + 1, [&](int i) { return i; });
 for (int q= Q; q--;) {
  if (D[q] == 1) {
   int i= seg1.max_right(0, [&](long long x) { return x <= X[q]; });
   seg2.apply(0, i, -2ll * L[q]);
  } else {
   int i= seg2.max_right(0, [&](long long x) { return x <= X[q]; });
   seg1.apply(i, N + 1, 2ll * L[q]);
  }
 }
 for (int i= 1; i <= N; ++i) cout << (seg1[i] - seg2[i]) / 2 << '\n';
 return 0;
}
