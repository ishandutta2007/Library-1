// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/UOA/UAPC/3024
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
// 永続性のverify

#include <iostream>
#include "src/DataStructure/WeightBalancedTree.hpp"
using namespace std;
struct RMQ {
 using T= int;
 static T op(T l, T r) { return min(l, r); }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 int A[N], B[N];
 for (int i= 0; i < N; i++) cin >> A[i];
 for (int i= 0; i < N; i++) cin >> B[i];
 using WBT= WeightBalancedTree<RMQ, false, true>;
 WBT wbt[2]= {WBT(A, A + N), WBT(B, B + N)};
 int Q;
 cin >> Q;
 for (int i= 0; i < Q; i++) {
  int x, y, z;
  cin >> x >> y >> z;
  if (x <= 2) {
   x--, y--;
   wbt[x].set(y, z);
  } else if (x <= 4) {
   x-= 3, y--;
   cout << wbt[x].prod(y, z) << '\n';
  } else {
   x-= 5;
   wbt[x]= wbt[!x];
  }
 }
 return 0;
}