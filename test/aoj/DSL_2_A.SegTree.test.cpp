// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/2/DSL_2_A
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/DataStructure/SegmentTree.hpp"
using namespace std;
struct RminQ {
 using T= int;
 static T ti() { return 0x7fffffff; }
 static T op(T l, T r) { return min(l, r); }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int n, q;
 cin >> n >> q;
 SegmentTree<RminQ> seg(n);
 while (q--) {
  int com, x, y;
  cin >> com >> x >> y;
  if (com) {
   cout << seg.prod(x, y + 1) << '\n';
  } else {
   seg.set(x, y);
  }
 }
 return 0;
}