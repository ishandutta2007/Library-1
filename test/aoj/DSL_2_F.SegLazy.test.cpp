// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/2/DSL_2_F
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/DataStructure/SegmentTree.hpp"
using namespace std;
struct RupdQ_RminQ {
 using T= int;
 using E= int;
 static T ti() { return 0x7fffffff; }
 static T op(T l, T r) { return min(l, r); }
 static bool mp(T& v, E f, int) { return v= f, true; }
 static void cp(E& pre, E suf) { pre= suf; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int n, q;
 cin >> n >> q;
 SegmentTree<RupdQ_RminQ> seg(n);
 while (q--) {
  int com, s, t;
  cin >> com >> s >> t;
  if (com) {
   cout << seg.prod(s, t + 1) << '\n';
  } else {
   int x;
   cin >> x;
   seg.apply(s, t + 1, x);
  }
 }
 return 0;
}
