#pragma once
#include "src/DataStructure/UnionFind.hpp"
#include "src/DataStructure/SkewHeap.hpp"
template <typename cost_t> class MinimumSpanningAborescense {
 struct Edge {
  std::size_t src, dst, id;
  cost_t cost;
  bool operator>(const Edge &r) const { return this->cost > r.cost; }
 };
 struct Op_Edge_add {
  using E= cost_t;
  static void mapping(Edge &l, const E &r) { l.cost+= r; }
  static void composition(E &l, const E &r) { l+= r; }
 };
 std::size_t n;
 std::vector<Edge> edges;
public:
 MinimumSpanningAborescense(std::size_t n): n(n) {}
 void add_edge(std::size_t src, std::size_t dst, cost_t cost) { edges.emplace_back(Edge{src, dst, edges.size(), cost}); }
 std::pair<cost_t, std::vector<Edge>> get_MSA(int root) {
  UnionFind uf(n);
  std::vector<SkewHeap<Edge, std::greater<Edge>, Op_Edge_add>> h(n);
  std::vector<Edge> es;
  for (auto &e: edges) h[e.dst].push(e);
  cost_t score= 0;
  std::size_t m= edges.size(), s= 0, u= s, cyc= 0, v;
  std::vector<int> seen(n, -1), paredge(m), ei, leaf(n, -1), par(n), usede(m);
  for (seen[root]= root; s < n; u= ++s, cyc= 0)
   for (std::vector<int> p, ch; seen[u] < 0; u= uf.root(v)) {
    if (p.push_back(u), seen[u]= s; h[u].empty()) return {-1, es};
    Edge min_e= h[u].pop();
    score+= min_e.cost, h[u].apply(-min_e.cost), ei.push_back(min_e.id);
    if (leaf[min_e.dst] == -1) leaf[min_e.dst]= min_e.id;
    for (; cyc; cyc--) paredge[ch.back()]= min_e.id, ch.pop_back();
    if (ch.push_back(min_e.id); seen[v= uf.root(min_e.src)] != s) continue;
    SkewHeap<Edge, std::greater<Edge>, Op_Edge_add> new_h;
    for (int w;;) {
     new_h+= h[w= p.back()];
     if (p.pop_back(), cyc++; !uf.unite(v, w)) break;
    }
    h[uf.root(v)]= new_h, seen[uf.root(v)]= -1;
   }
  for (auto it= ei.rbegin(); it != ei.rend(); it++) {
   if (usede[*it]) continue;
   es.emplace_back(edges[*it]);
   for (int x= leaf[edges[*it].dst]; x != *it; x= paredge[x]) usede[x]= 1;
  }
  return {score, es};
 }
};