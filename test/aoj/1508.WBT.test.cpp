// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/UOA/UAPC/1508
// competitive-verifier: TLE 1
// split3, push_front, pop_back, operator+, prod, set の verify

#include <iostream>
#include <vector>
#include "src/DataStructure/WeightBalancedTree.hpp"
using namespace std;
struct RminQ {
 using T= int;
 static T op(T l, T r) { return min(l, r); }
};
int main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 vector<int> A(N);
 for (int i= 0; i < N; i++) cin >> A[i];
 WeightBalancedTree<RminQ> wbt(A);
 while (Q--) {
  int x, y, z;
  cin >> x >> y >> z;
  if (x == 0) {
   auto [a, b, c]= wbt.split3(y, z + 1);
   b.push_front(b.pop_back());
   wbt= a + b + c;
  } else if (x == 1) cout << wbt.prod(y, z + 1) << '\n';
  else wbt.set(y, z);
 }
}