#define PROBLEM "https://judge.yosupo.jp/problem/vertex_add_subtree_sum"
#include <iostream>
#include <vector>
#include "src/Graph/Graph.hpp"
#include "src/Graph/HeavyLightDecomposition.hpp"
#include "src/DataStructure/BinaryIndexedTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 vector<long long> a(N);
 for (int u= 0; u < N; ++u) cin >> a[u];
 Graph g(N);
 for (int i= 1, p; i < N; ++i) cin >> p, g.add_edge(p, i);
 HeavyLightDecomposition hld(g.adjacency_vertex(1), 0);
 BinaryIndexedTree<long long> bit([&]() {
  vector<long long> v(N);
  for (int u= N; u--;) v[hld.to_seq(u)]= a[u];
  return v;
 }());
 while (Q--) {
  bool op;
  int u;
  cin >> op >> u;
  if (op) {
   auto [l, r]= hld.subtree(u);
   cout << bit.sum(l, r) << '\n';
  } else {
   int x;
   cin >> x;
   bit.add(hld.to_seq(u), x);
  }
 }
 return 0;
}
