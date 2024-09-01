// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/2/DSL_2_D
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/DataStructure/SegmentTree_Dual.hpp"
using namespace std;
struct RupdQ {
 using T= int;
 using E= int;
 static void mp(T& v, E f) { v= f; }
 static void cp(E& pre, E suf) { pre= suf; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int n, q;
 cin >> n >> q;
 SegmentTree_Dual<RupdQ> seg(n, 0x7fffffff);
 while (q--) {
  bool com;
  cin >> com;
  if (com) {
   int i;
   cin >> i;
   cout << seg[i] << endl;
  } else {
   int s, t, x;
   cin >> s >> t >> x;
   seg.apply(s, t + 1, x);
  }
 }
 return 0;
}