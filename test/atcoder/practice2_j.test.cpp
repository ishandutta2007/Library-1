// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/practice2/tasks/practice2_j
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// max_right
#include <iostream>
#include <vector>
#include "src/DataStructure/SegmentTree.hpp"
using namespace std;
struct M {
 using T= int;
 static T ti() { return -(1 << 30); }
 static T op(T l, T r) { return max(l, r); }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, Q;
 cin >> N >> Q;
 vector<int> A(N);
 for (int i= 0; i < N; ++i) cin >> A[i];
 SegmentTree<M> seg(A);
 while (Q--) {
  int t, a, b;
  cin >> t >> a >> b, --a;
  if (t == 1) seg.set(a, b);
  else if (t == 2) cout << seg.prod(a, b) << '\n';
  else cout << seg.max_right(a, [&](int x) { return x < b; }) + 1 << '\n';
 }
 return 0;
}
