// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/UOA/UAPC/3024
// competitive-verifier: TLE 1
// 永続化のverify
#include <iostream>
#include "src/DataStructure/SegmentTree_Dynamic.hpp"
using namespace std;
struct RMQ {
 using T= int;
 static T op(T l, T r) { return min(l, r); }
 static T ti() { return 1 << 30; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 int A[N], B[N];
 for (int i= 0; i < N; i++) cin >> A[i];
 for (int i= 0; i < N; i++) cin >> B[i];
 using Seg= SegmentTree_Dynamic<RMQ, true>;
 Seg seg[2]= {Seg(A, A + N), Seg(B, B + N)};
 int Q;
 cin >> Q;
 for (int i= 0; i < Q; i++) {
  int x, y, z;
  cin >> x >> y >> z;
  if (x <= 2) {
   x--, y--;
   seg[x].set(y, z);
  } else if (x <= 4) {
   x-= 3, y--;
   cout << seg[x].prod(y, z) << '\n';
  } else {
   x-= 5;
   seg[x]= seg[!x];
  }
 }
 return 0;
}