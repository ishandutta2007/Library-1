#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/challenges/sources/UOA/UAPC/1508"
#include <iostream>
#include <vector>
#include "src/DataStructure/WeightBalancedTree.hpp"
using namespace std;
// 平衡二分木のverify

struct RminQ {
 using T= int;
 static T op(const T &l, const T &r) { return min(l, r); }
};
int main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using WBT= WeightBalancedTree<RminQ>;
 int N, Q;
 cin >> N >> Q;
 vector<int> A(N);
 for (int i= 0; i < N; i++) cin >> A[i];
 WBT wbt(A);
 while (Q--) {
  int x, y, z;
  cin >> x >> y >> z;
  if (x == 0) {
   auto [a, b, c]= wbt.split3(y, z + 1);
   b.push_front(b.pop_back());
   wbt= a + b + c;
  } else if (x == 1) cout << wbt.fold(y, z + 1) << '\n';
  else wbt.set(y, z);
  if (WBT::percentage_used() > 97) wbt.rebuild();
 }
}