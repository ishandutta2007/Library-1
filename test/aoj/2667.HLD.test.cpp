// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/2667
// competitive-verifier: TLE 1
// パス総和、部分木加算
#include <iostream>
#include "src/Graph/Graph.hpp"
#include "src/Graph/HeavyLightDecomposition.hpp"
#include "src/DataStructure/BinaryIndexedTree_RangeAdd.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 Graph g(N, N - 1);
 for (int i= 0; i < N - 1; ++i) cin >> g[i];
 HeavyLightDecomposition tree(g, 0);
 BinaryIndexedTree_RangeAdd<long long> bit(N);
 while (Q--) {
  int op, a, b;
  cin >> op >> a >> b;
  if (op) {
   auto [l, r]= tree.subtree(a);
   bit.add_range(l + 1, r, b);
  } else {
   long long ans= 0;
   for (auto [x, y]: tree.path(a, b, true)) ans+= x <= y ? bit.sum(x, y + 1) : bit.sum(y, x + 1);
   cout << ans << '\n';
  }
 }
 return 0;
}