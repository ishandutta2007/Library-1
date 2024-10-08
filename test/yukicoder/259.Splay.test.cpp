// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/259
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
// reverse, split, + の verify

#include <iostream>
#include "src/DataStructure/SplayTree.hpp"
using namespace std;
struct RSQ {
 using T= long long;
 static T op(T a, T b) { return a + b; }
 using commute= void;
};
int main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 SplayTree<RSQ, true> stL(N, 0), stR(N, 0);
 int now= 0;
 while (Q--) {
  char x;
  int t, y, z;
  cin >> x >> t >> y >> z;
  int dt= t - now;
  dt%= 2 * N;
  if (dt > N) swap(stL, stR), stL.reverse(), stR.reverse(), dt-= N;
  auto [Ll, Lr]= stL.split(dt);
  auto [Rl, Rr]= stR.split(N - dt);
  Ll.reverse(), Rr.reverse();
  stL= Lr + Rr;
  stR= Ll + Rl;
  if (x == 'L') {
   stL.mul(y, z);
  } else if (x == 'R') {
   stR.mul(y, z);
  } else {
   cout << stL.prod(y, z) + stR.prod(y, z) << '\n';
  }
  now= t;
 }
 return 0;
}