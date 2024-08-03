// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/UOA/UAPC/3024

// 永続化のverify

#include <iostream>
#include "src/DataStructure/SegmentTree_Patricia.hpp"
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
 SegmentTree_Patricia<RMQ, true> seg[2]= {SegmentTree_Patricia<RMQ, true>(A, A + N), SegmentTree_Patricia<RMQ, true>(B, B + N)};
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