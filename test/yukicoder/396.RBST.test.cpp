// competitive-verifier: PROBLEM https://yukicoder.me/problems/396

// reverse, split, + の verify

#include <iostream>
#include "src/DataStructure/RandomizedBinarySearchTree.hpp"
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
 RandomizedBinarySearchTree<RSQ, true> rbstL(N, 0), rbstR(N, 0);
 int now= 0;
 while (Q--) {
  char x;
  int t, y, z;
  cin >> x >> t >> y >> z;
  int dt= t - now;
  dt%= 2 * N;
  if (dt > N) swap(rbstL, rbstR), dt-= N;
  auto [Ll, Lr]= rbstL.split(dt);
  auto [Rl, Rr]= rbstR.split(N - dt);
  Ll.reverse(), Rr.reverse();
  rbstL= Lr + Rr;
  rbstR= Ll + Rl;
  if (x == 'L') {
   rbstL.mul(y, z);
  } else if (x == 'R') {
   rbstR.mul(y, z);
  } else {
   cout << rbstL.prod(y, z) + rbstR.prod(y, z) << '\n';
  }
  now= t;
 }
 return 0;
}