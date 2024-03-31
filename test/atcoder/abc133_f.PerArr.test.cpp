#define PROBLEM "https://atcoder.jp/contests/abc133/tasks/abc133_f"

// 永続配列(at) の verify

#include <iostream>
#include <vector>
#include "src/DataStructure/PersistentArray.hpp"
#include "src/Graph/Graph.hpp"
#include "src/Graph/HeavyLightDecomposition.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 Graph g(N, N - 1);
 vector<int> c(N - 1), d(N - 1);
 for (int i= 0; i < N - 1; ++i) cin >> g[i] >> c[i] >> d[i], --g[i];
 HeavyLightDecomposition hld(g, 0);
 using PerArr= PersistentArray<int, 8>;
 vector<PerArr> Arr1(N), Arr2(N);
 vector<int> dep(N);
 auto adj= g.adjacency_edge(0);
 for (int i= 0; i < N; ++i) {
  int v= hld.to_vertex(i), p= hld.parent(v);
  for (int e: adj[v]) {
   int u= g[e].to(v);
   if (u == p) continue;
   Arr1[u]= Arr1[v], Arr2[u]= Arr2[v];
   Arr1[u][c[e]]+= 1, Arr2[u][c[e]]+= d[e];
   dep[u]= dep[v] + d[e];
  }
 }
 for (int i= 0; i < Q; i++) {
  int x, y, u, v;
  cin >> x >> y >> u >> v;
  int lca= hld.lca(--u, --v);
  cout << dep[u] + dep[v] - 2 * dep[lca] + y * (Arr1[u].get(x) + Arr1[v].get(x) - 2 * Arr1[lca].get(x)) - (Arr2[u].get(x) + Arr2[v].get(x) - 2 * Arr2[lca].get(x)) << '\n';
 }
 return 0;
}