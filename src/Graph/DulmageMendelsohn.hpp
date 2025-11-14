#pragma once
#include <algorithm>
#include <numeric>
#include "src/Graph/BipartiteGraph.hpp"
class DulmageMendelsohn {
 int L;
 std::vector<int> b, m, a;
 CSRArray<int> dag[2];
public:
 DulmageMendelsohn(const BipartiteGraph &bg): L(bg.left_size()) {
  auto adj= bg.adjacency_vertex(0);
  const int n= adj.size();
  m.assign(n, -1), b.assign(n, -3), a= _bg_internal::_bm(L, adj, m);
  std::vector<int> q(n - L);
  int t= 0, k= 0;
  for (int l= L; l--;)
   if (a[l] != -1)
    if (b[l]= -1; m[l] != -1) b[m[l]]= -1;
  for (int r= n; r-- > L;)
   if (m[r] == -1) b[q[t++]= r]= 0;
  for (int i= 0, r, w; i < t; ++i)
   for (int l: adj[r= q[i]])
    if (b[l] == -3)
     if (b[l]= 0, w= m[l]; w != -1 && b[w] == -3) b[q[t++]= w]= 0;
  t= 0;
  {
   std::vector<int> c(adj.p.begin(), adj.p.begin() + L);
   for (int l= L; l--;)
    if (int v= l; b[v] == -3)
     for (b[v]= -2; v >= 0;) {
      if (c[v] == adj.p[v + 1]) a[t++]= v, v= b[v];
      else if (int w= m[adj.dat[c[v]++]]; b[w] == -3) b[w]= v, v= w;
     }
  }
  for (int i= 0, e= 0, r; t--;)
   if (int s= a[t], p= m[s]; b[p] == -3)
    for (b[q[e++]= p]= b[s]= ++k; i < e; ++i)
     for (int l: adj[r= q[i]])
      if (b[m[l]] == -3) b[q[e++]= m[l]]= b[l]= k;
  ++k;
  for (int l= L; l--;)
   if (b[l] == -1)
    if (b[l]= k; m[l] != -1) b[m[l]]= k;
  a.assign(k + 2, 0);
  for (int i= n; i--;) ++a[b[i]];
  for (int i= 0; i <= k; ++i) a[i + 1]+= a[i];
  for (int i= n; i--;) m[--a[b[i]]]= i;
  Graph h(k + 1);
  for (auto [l, r]: bg)
   if (b[l] != b[r]) h.add_edge(b[l], b[r]);
  std::sort(h.begin(), h.end()), h.erase(std::unique(h.begin(), h.end()), h.end()), dag[0]= h.adjacency_vertex(1), dag[1]= h.adjacency_vertex(-1);
 }
 size_t size() const { return a.size() - 1; }
 ConstListRange<int> block(int k) const { return {m.cbegin() + a[k], m.cbegin() + a[k + 1]}; }
 int operator()(int i) const { return b[i]; }
 std::vector<int> min_vertex_cover(std::vector<int> ord= {}) const {
  if (ord.empty()) ord.resize(b.size()), std::iota(ord.begin(), ord.end(), 0);
  std::vector<char> z(size(), -1);
  std::vector<int> q(size()), vc;
  z[0]= 1, z.back()= 0;
  for (int v: ord) {
   int c= (v >= L), k= b[v], s= z[k];
   if (s == -1) {
    auto &adj= dag[z[q[0]= k]= s= !c];
    for (int i= 0, t= 1; i < t; ++i)
     for (int u: adj[q[i]])
      if (z[u] == -1) z[u]= s, q[t++]= u;
   }
   if (c ^ s) vc.push_back(v);
  }
  return vc;
 }
};