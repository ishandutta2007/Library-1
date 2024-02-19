#pragma once
#include <cassert>
#include <tuple>
#include <algorithm>
#include "src/Graph/Graph.hpp"
// [0, L) is left, [L, n) is right
struct BipartiteGraph: Graph {
 size_t L;
 BipartiteGraph() {}
 BipartiteGraph(size_t L, size_t R, size_t m= 0): Graph(L + R, m), L(L) {}
 size_t left_size() const { return L; }
 size_t right_size() const { return this->n - L; }
};
std::vector<int> paint_in_2_color(const CSRArray<int> &adj) {
 const int n= adj.size();
 std::vector<int> col(n, -1);
 for (int s= n; s--;)
  if (col[s] == -1) {
   std::vector<int> q= {s};
   for (int i= col[s]= 0, v; i < (int)q.size(); ++i)
    for (int u: adj[v= q[i]])
     if (int c= col[v]; col[u] == c) return {};
     else if (col[u] == -1) col[u]= c ^ 1, q.push_back(u);
  }
 return col;
}
std::vector<int> paint_in_2_color(const Graph &g) { return paint_in_2_color(g.adjacency_vertex(0)); }
// { BipartiteGraph , original to new, new to original }
// {{},{},{}} if not bipartite
std::tuple<BipartiteGraph, std::vector<int>, std::vector<int>> graph_to_bipartite(const Graph &g, std::vector<int> color= {}) {
 if (color.empty()) color= paint_in_2_color(g);
 if (color.empty()) return {};
 const int n= g.vertex_size(), m= g.edge_size();
 std::vector<int> a(n), b(n);
 int l= 0, r= n;
 for (int i= n; i--;) b[a[i]= color[i] ? --r : l++]= i;
 BipartiteGraph bg(l, n - l, m);
 for (int i= m; i--;) {
  auto [u, v]= g[i];
  bg[i]= std::minmax(a[u], a[v]);
 }
 return {bg, a, b};
}
namespace _bg_internal {
std::vector<int> _bm(int L, const CSRArray<int> &adj, std::vector<int> &m) {
 std::vector<int> a, p, q(L);
 for (bool u= true; u;) {
  u= false, a.assign(L, -1), p.assign(L, -1);
  int t= 0;
  for (int l= L; l--;)
   if (m[l] == -1) q[t++]= a[l]= p[l]= l;
  for (int i= 0; i < t; ++i)
   if (int l= q[i], x; m[a[l]] == -1)
    for (int r: adj[l]) {
     if (x= m[r]; x == -1) {
      for (u= true; r != -1; l= p[l]) m[r]= l, std::swap(m[l], r);
      break;
     }
     if (p[x] == -1) a[q[t++]= x]= a[p[x]= l];
    }
 }
 return a;
}
}
template <bool lexical= false> std::pair<std::vector<int>, std::vector<int>> bipartite_matching(const BipartiteGraph &g, std::vector<int> partner= {}) {
 const int L= g.left_size(), M= g.edge_size();
 if (partner.empty()) partner.assign(g.vertex_size(), -1);
 assert(partner.size() == g.vertex_size());
 {
  CSRArray<int> adj{std::vector<int>(M), std::vector<int>(L + 1)};
  for (auto [l, r]: g) ++adj.p[l];
  for (int i= 0; i < L; ++i) adj.p[i + 1]+= adj.p[i];
  for (auto [l, r]: g) adj.dat[--adj.p[l]]= r;
  if constexpr (lexical) {
   for (int l= L; l--;) std::sort(adj[l].begin(), adj[l].end());
   _bg_internal::_bm(L, adj, partner);
   std::vector<char> a(L, 1);
   for (int l= 0; l < L; ++l)
    if (int r= partner[l], v= l; r != -1) {
     std::vector<int> p(L, partner[v]= partner[r]= -1), c(adj.p.begin(), adj.p.begin() + L);
     for (p[v]= -2;;) {
      if (c[v] == adj.p[v + 1]) v= p[v];
      else if (int u= partner[r= adj.dat[c[v]++]]; u == -1) {
       for (; r != -1; v= p[v]) partner[r]= v, std::swap(partner[v], r);
       break;
      } else if (a[u] && p[u] == -1) p[u]= v, v= u;
     }
     a[l]= 0;
    }
  } else _bg_internal::_bm(L, adj, partner);
 }
 std::vector<int> c;
 std::vector<char> p(L);
 for (int i= 0; i < M; ++i)
  if (auto [l, r]= g[i]; partner[l] == r && !p[l]) c.push_back(i), p[l]= 1;
 return {c, partner};
}