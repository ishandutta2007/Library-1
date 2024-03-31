#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/challenges/sources/UOA/UAPC/1508"

// split3, push_front, pop_back, operator+, fold, set の verify

#include <iostream>
#include <vector>
#include "src/DataStructure/RandomizedBinarySearchTree.hpp"
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
 RandomizedBinarySearchTree<RminQ> rbst(A);
 while (Q--) {
  int x, y, z;
  cin >> x >> y >> z;
  if (x == 0) {
   auto [a, b, c]= rbst.split3(y, z + 1);
   b.push_front(b.pop_back());
   rbst= a + b + c;
  } else if (x == 1) cout << rbst.fold(y, z + 1) << '\n';
  else rbst.set(y, z);
 }
}