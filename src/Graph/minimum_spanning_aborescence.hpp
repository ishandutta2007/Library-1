#pragma once
#include <utility>
#include "src/Graph/Graph.hpp"
#include "src/DataStructure/UnionFind.hpp"
#include "src/DataStructure/UnionFind_Undoable.hpp"
// return {total cost, edge ids}
// return {0, {}} if the graph has no spanning aborescence of the root
template <class cost_t> std::pair<cost_t, std::vector<int>> minimum_spanning_aborescence(const Graph &g, std::vector<cost_t> w, int root) {
 const int n= g.vertex_size(), m= g.edge_size();
 assert((int)w.size() == m);
 std::vector<cost_t> lz(m);
 std::vector<std::pair<int, int>> lr(m, {-1, -1}), cyc;
 std::vector<int> top(n, -1), es(n, -1);
 UnionFind uf(n);
 UnionFind_Undoable uf2(n);
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
   if (top[v] == -1) return {};
   int x= uf2.leader(g[es[v]= top[v]].first);
   if (sum+= w[es[v]], upd(es[v], w[es[v]]); uf.unite(v, x)) break;
   int t= uf2.time();
   for (int r; uf2.unite(v, x); v= r, x= uf2.leader(g[es[x]].first)) top[r= uf2.leader(v)]= merge(merge, top[v], top[x]);
   cyc.emplace_back(es[v], t);
   while (top[v] != -1 && uf2.connected(v, g[top[v]].first)) {
    auto [l, r]= lr[top[v]];
    push(top[v]), top[v]= merge(merge, l, r);
   }
  }
 }
 for (auto it= cyc.rbegin(); it != cyc.rend(); ++it) {
  auto [e, t]= *it;
  int r= uf2.leader(g[e].second);
  uf2.rollback(t);
  int v= uf2.leader(g[es[r]].second);
  es[v]= std::exchange(es[r], e);
 }
 es.erase(es.begin() + root);
 return {sum, es};
}