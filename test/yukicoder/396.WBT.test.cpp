// competitive-verifier: PROBLEM https://yukicoder.me/problems/396
// competitive-verifier: TL 1

// reverse, split, + の verify

#include <iostream>
#include "src/DataStructure/WeightBalancedTree.hpp"
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
 WeightBalancedTree<RSQ, true> wbtL(N, 0), wbtR(N, 0);
 int now= 0;
 while (Q--) {
  char x;
  int t, y, z;
  cin >> x >> t >> y >> z;
  int dt= t - now;
  dt%= 2 * N;
  if (dt > N) swap(wbtL, wbtR), wbtL.reverse(), wbtR.reverse(), dt-= N;
  auto [Ll, Lr]= wbtL.split(dt);
  auto [Rl, Rr]= wbtR.split(N - dt);
  Ll.reverse(), Rr.reverse();
  wbtL= Lr + Rr;
  wbtR= Ll + Rl;
  if (x == 'L') {
   wbtL.mul(y, z);
  } else if (x == 'R') {
   wbtR.mul(y, z);
  } else {
   cout << wbtL.prod(y, z) + wbtR.prod(y, z) << '\n';
  }
  now= t;
 }
 return 0;
}