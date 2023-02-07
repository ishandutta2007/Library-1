#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2667"
// パス総和、部分木加算
#include <iostream>
#include "src/Graph/Tree.hpp"
#include "src/DataStructure/BinaryIndexedTree_RangeAdd.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 Tree tree(N);
 for (int i= 0; i < N - 1; ++i) {
  int a, b;
  cin >> a >> b;
  tree.add_edge(a, b);
 }
 tree.build(0);
 BinaryIndexedTree_RangeAdd<int> bit(N);
 while (Q--) {
  int op, a, b;
  cin >> op >> a >> b;
  if (op) {
   auto [l, r]= tree.subtree(a);
   bit.add_range(l + 1, r, b);
  } else {
   int ans= 0;
   for (auto [x, y]: tree.path<true>(a, b)) ans+= x <= y ? bit.sum(x, y + 1) : bit.sum(y, x + 1);
   cout << ans << '\n';
  }
 }
 return 0;
}