#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/2/DSL_2_D"
#include <iostream>
#include "src/DataStructure/SegmentTree_Dual.hpp"
using namespace std;
struct RupdQ {
 using T= int;
 using E= int;
 static void mapping(T& v, const E& f) { v= f; }
 static void composition(E& pre, const E& suf) { pre= suf; }
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