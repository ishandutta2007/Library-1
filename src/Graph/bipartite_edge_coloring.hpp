#pragma once
#include <queue>
#include <numeric>
#include "src/DataStructure/UnionFind.hpp"
#include "src/Graph/BipartiteGraph.hpp"
std::vector<int> bipartite_edge_coloring(BipartiteGraph g) {
 const int m= g.edge_size();
 int L= g.left_size(), n= g.vertex_size(), D, col= 0;
 {
  std::vector<int> deg(n), id(n);
  for (auto [l, r]: g) ++deg[l], ++deg[r];
  D= *std::max_element(deg.begin(), deg.end());
  UnionFind uf(n);
  for (int _: {0, n}) {
   auto [b, e]= std::minmax(_, L);
   std::priority_queue<std::pair<int, int>> pq;
   for (int i= b; i < e; ++i) pq.emplace(-deg[i], i);
   for (; pq.size() > 1;) {
    auto [a, v]= pq.top();
    pq.pop();
    auto [b, u]= pq.top();
    pq.pop();
    if (int sum= a + b; -sum <= D) uf.unite(v, u), pq.emplace(sum, v);
    else break;
   }
  }
  int i= 0, cl= 0, cr= 0;
  for (; i < L; ++i)
   if (uf.root(i) == i) id[i]= cl++;
  for (; i < n; ++i)
   if (uf.root(i) == i) id[i]= cr++;
  L= std::max(cl, cr), deg.assign(n= L + L, 0), g.reserve(L * D);
  for (auto &[l, r]: g) ++deg[l= id[uf.root(l)]], ++deg[r= id[uf.root(r)] + L];
  for (int l= 0, r= L; l < L; ++l)
   while (deg[l] < D) {
    while (r < n && deg[r] == D) ++r;
    int x= D - std::max(deg[l], deg[r]);
    for (int k= x; k--;) g.add_edge(l, r);
    deg[l]+= x, deg[r]+= x;
   }
 }
 std::vector<int> color(m, -1);
 auto rc= [&](auto &&rc, int d, const std::vector<int> &idx) -> void {
  if (!d) return;
  if (d == 1) {
   for (int e: idx)
    if (e < m) color[e]= col;
   ++col;
   return;
  }
  if (d & 1) {
   CSRArray<int> adj{std::vector<int>(idx.size()), std::vector<int>(L + 1)};
   for (int e: idx) ++adj.p[g[e].first];
   for (int i= 0; i < L; ++i) adj.p[i + 1]+= adj.p[i];
   for (int e: idx) {
    auto [l, r]= g[e];
    adj.dat[--adj.p[l]]= r;
   }
   std::vector<int> mate(n, -1), rm;
   _bg_internal::_bm(L, adj, mate);
   for (int e: idx) {
    auto [l, r]= g[e];
    if (mate[l] == r) {
     if (mate[l]= mate[r]= -1; e < m) color[e]= col;
    } else rm.push_back(e);
   }
   return ++col, rc(rc, d - 1, rm);
  }
  const int mm= idx.size();
  std::vector<int> circuit;
  {
   std::vector<int> c(mm), p(n + 1);
   for (int e: idx) {
    auto [l, r]= g[e];
    ++p[l], ++p[r];
   }
   for (int i= 0; i < L; ++i) p[i + 1]+= p[i];
   for (int i= mm; i--;) {
    auto [l, r]= g[idx[i]];
    c[--p[l]]= i, c[--p[r]]= i;
   }
   std::vector<int> it(p.begin(), p.begin() + n);
   std::vector<char> used1(n), used2(mm);
   for (int v= n; v--;)
    if (!used1[v]) {
     for (std::vector<std::pair<int, int>> st= {{v, -1}}; st.size();) {
      auto [u, e]= st.back();
      if (used1[u]= 1; it[u] == p[u + 1]) circuit.push_back(e), st.pop_back();
      else if (int i= c[it[u]++]; !used2[i]) used2[i]= 1, st.emplace_back(g[idx[i]].to(u), i);
     }
     circuit.pop_back();
    }
  }
  std::vector<int> half1(mm / 2), half2(mm / 2);
  for (int i= mm / 2; i--;) half1[i]= idx[circuit[i * 2]], half2[i]= idx[circuit[i * 2 + 1]];
  rc(rc, d / 2, half1), rc(rc, d / 2, half2);
 };
 std::vector<int> idx(m * D);
 return std::iota(idx.begin(), idx.end(), 0), rc(rc, D, idx), color;
}