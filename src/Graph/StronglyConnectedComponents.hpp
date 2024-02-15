#pragma once
#include <algorithm>
#include "src/Graph/Graph.hpp"
class StronglyConnectedComponents {
 Graph _g;
 std::vector<int> csr, pos, ord;
public:
 StronglyConnectedComponents(const Graph &g): _g(g) {
  const int n= g.vertex_size();
  csr.assign(n, -2), ord.resize(n), pos= {0};
  {
   auto adj= g.adjacency_vertex(1);
   std::vector<int> ei(adj.p.begin(), adj.p.begin() + n);
   for (int s= 0, k= n, p; s < n; ++s)
    if (csr[s] == -2)
     for (csr[p= s]= -1; p >= 0;)
      if (ei[p] == adj.p[p + 1]) ord[--k]= p, p= csr[p];
      else if (int q= adj.dat[ei[p]++]; csr[q] == -2) csr[q]= p, p= q;
  }
  for (auto &[s, d]: _g) std::swap(s, d);
  auto adj= _g.adjacency_vertex(1);
  std::vector<char> vis(n);
  int k= 0, p= 0;
  for (int s: ord)
   if (!vis[s]) {
    for (vis[csr[k++]= s]= 1; p < k; ++p)
     for (int u: adj[csr[p]])
      if (!vis[u]) vis[csr[k++]= u]= 1;
    pos.push_back(k);
   }
  for (int i= pos.size() - 1; i--;)
   while (k > pos[i]) ord[csr[--k]]= i;
 }
 size_t size() const { return pos.size() - 1; }
 ConstListRange<int> block(int k) const { return {csr.cbegin() + pos[k], csr.cbegin() + pos[k + 1]}; }
 int operator()(int i) const { return ord[i]; }
 Graph dag() const {
  Graph ret(size());
  for (auto [d, s]: _g)
   if (int u= (*this)(s), v= (*this)(d); u != v) ret.add_edge(u, v);
  return sort(ret.begin(), ret.end()), ret.erase(unique(ret.begin(), ret.end()), ret.end()), ret;
 }
};