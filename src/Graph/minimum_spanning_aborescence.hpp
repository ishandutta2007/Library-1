#pragma once
#include <utility>
#include "src/Graph/Graph.hpp"
#include "src/DataStructure/UnionFind.hpp"
// return edge ids of minimum spanning aborescence
template <class cost_t> std::pair<cost_t, std::vector<int>> minimum_spanning_aborescence(const Graph &g, std::vector<cost_t> w, int root) {
 const int n= g.vertex_size(), m= g.edge_size();
 assert((int)w.size() == m);
 std::vector<cost_t> lz(m);
 std::vector<std::pair<int, int>> lr(m, {-1, -1}), cycles;
 std::vector<int> top(n, -1), edge(n, -1);
 UnionFind uf(n);
 UnionFind<true> uf2(n);
 auto upd= [&](int i, cost_t v) { w[i]-= v, lz[i]+= v; };
 auto push= [&](int i) {
  auto [l, r]= lr[i];
  if (l != -1) upd(l, lz[i]);
  if (r != -1) upd(r, lz[i]);
  lz[i]= 0;
 };
 auto merge= [&](auto &&rec, int u, int v) -> int {
  if (u == -1) return v;
  if (v == -1) return u;
  if (w[v] < w[u]) std::swap(u, v);
  auto &[l, r]= lr[u];
  return push(u), r= rec(rec, r, v), std::swap(l, r), u;
 };
 for (int i= m; i--;) {
  auto [s, d]= g[i];
  top[d]= merge(merge, top[d], i);
 }
 cost_t sum= 0;
 for (int i= n; i--;) {
  if (i == root) continue;
  for (int v= i;;) {
   if (top[v] == -1) return {cost_t(), std::vector<int>()};
   int nxt= uf2.root(g[edge[v]= top[v]].first);
   if (sum+= w[edge[v]], upd(edge[v], w[edge[v]]); uf.unite(v, nxt)) break;
   int t= uf2.time();
   for (int r; uf2.unite(v, nxt); v= r, nxt= uf2.root(g[edge[nxt]].first)) top[r= uf2.root(v)]= merge(merge, top[v], top[nxt]);
   cycles.emplace_back(edge[v], t);
   while (top[v] != -1 && uf2.same(v, g[top[v]].first)) {
    auto [l, r]= lr[top[v]];
    push(top[v]), top[v]= merge(merge, l, r);
   }
  }
 }
 for (auto it= cycles.rbegin(); it != cycles.rend(); ++it) {
  auto [e, t]= *it;
  int r= uf2.root(g[e].second);
  uf2.rollback(t);
  int v= uf2.root(g[edge[r]].second);
  edge[v]= std::exchange(edge[r], e);
 }
 edge.erase(edge.begin() + root);
 return {sum, edge};
}