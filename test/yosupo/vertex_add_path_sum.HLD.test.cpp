// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/vertex_add_path_sum
// competitive-verifier: TLE 0.5
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
 Graph g(N, N - 1);
 for (int i= 0; i < N - 1; ++i) cin >> g[i];
 HeavyLightDecomposition hld(g);
 BinaryIndexedTree<long long> bit([&]() {
  vector<long long> v(N);
  for (int u= N; u--;) v[hld.to_seq(u)]= a[u];
  return v;
 }());
 while (Q--) {
  bool op;
  cin >> op;
  if (op) {
   int u, v;
   cin >> u >> v;
   long long ans= 0;
   for (auto [x, y]: hld.path(u, v)) ans+= x < y ? bit.sum(x, y + 1) : bit.sum(y, x + 1);
   cout << ans << '\n';
  } else {
   int p, x;
   cin >> p >> x;
   bit.add(hld.to_seq(p), x);
  }
 }
 return 0;
}