#define PROBLEM "https://yukicoder.me/problems/no/1600"
#include <iostream>
#include <vector>
#include <array>
#include <tuple>

#include "src/Math/ModInt.hpp"
#include "src/DataStructure/UnionFind.hpp"
#include "src/Graph/Graph.hpp"
#include "src/Graph/HeavyLightDecomposition.hpp"
#include "src/DataStructure/SegmentTree_2D.hpp"
using namespace std;
struct RMQ {
 using T= int;
 static T ti() { return 0x7fffffff; }
 static T op(T a, T b) { return min(a, b); }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<1000000007>;
 int N, M;
 cin >> N >> M;
 vector<Edge> es(M);
 for (int i= 0; i < M; ++i) cin >> es[i], --es[i];
 Graph g(N);
 vector<Mint> C;
 Mint w= 1;
 UnionFind uf(N);
 vector<char> used(M);
 for (int i= 0; i < M; ++i) {
  auto [A, B]= es[i];
  w+= w;
  if (uf.unite(A, B)) {
   used[i]= true;
   g.add_edge(A, B), C.push_back(w);
  }
 }
 HeavyLightDecomposition tree(g);
 auto adj= g.adjacency_edge(0);
 vector<Mint> dep(N);
 for (int i= 0, v; i < N; ++i)
  for (int e: adj[v= tree.to_vertex(i)])
   if (int u= g[e].to(v); u != tree.parent(v)) dep[u]= dep[v] + C[e];
 auto dist= [&](int u, int v) { return dep[u] + dep[v] - dep[tree.lca(u, v)] * 2; };
 vector<array<int, 3>> xyw;
 for (int i= 0; i < M; ++i) {
  if (used[i]) continue;
  auto [A, B]= es[i];
  int a= tree.to_seq(A), b= tree.to_seq(B);
  if (a > b) swap(a, b);
  xyw.push_back({a, b, i});
 }
 SegmentTree_2D<int, RMQ> seg(xyw);
 int Q;
 cin >> Q;
 while (Q--) {
  int u, v, e;
  cin >> u >> v >> e, --u, --v, --e;
  auto [x, y]= es[e];
  if (tree.parent(y) == x) swap(x, y);
  bool u_in= tree.in_subtree(u, x);
  if (!used[e] || u_in == tree.in_subtree(v, x)) {
   cout << dist(u, v) << '\n';
   continue;
  }
  auto [l, r]= tree.subtree(x);
  int i= min(seg.fold(0, l, l, r), seg.fold(l, r, r, N));
  if (i > M) {
   cout << -1 << '\n';
   continue;
  }
  auto [p, q]= es[i];
  if (!u_in) swap(u, v);
  if (tree.in_subtree(q, x)) swap(p, q);
  cout << dist(u, p) + dist(v, q) + Mint(2).pow(i + 1) << '\n';
 }
 return 0;
}