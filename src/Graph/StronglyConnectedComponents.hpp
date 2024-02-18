#pragma once
#include <algorithm>
#include "src/Graph/Graph.hpp"
class StronglyConnectedComponents {
 std::vector<int> m, q, b;
public:
 StronglyConnectedComponents(const Graph &g) {
  const int n= g.vertex_size();
  m.assign(n, -2), b.resize(n), q= {0};
  {
   auto adj= g.adjacency_vertex(1);
   std::vector<int> c(adj.p.begin(), adj.p.begin() + n);
   for (int s= 0, k= n, p; s < n; ++s)
    if (m[s] == -2)
     for (m[p= s]= -1; p >= 0;) {
      if (c[p] == adj.p[p + 1]) b[--k]= p, p= m[p];
      else if (int w= adj.dat[c[p]++]; m[w] == -2) m[w]= p, p= w;
     }
  }
  auto adj= g.adjacency_vertex(-1);
  std::vector<char> z(n);
  int k= 0, p= 0;
  for (int s: b)
   if (!z[s]) {
    for (z[m[k++]= s]= 1; p < k; ++p)
     for (int u: adj[m[p]])
      if (!z[u]) z[m[k++]= u]= 1;
    q.push_back(k);
   }
  for (int i= q.size() - 1; i--;)
   while (k > q[i]) b[m[--k]]= i;
 }
 size_t size() const { return q.size() - 1; }
 ConstListRange<int> block(int k) const { return {m.cbegin() + q[k], m.cbegin() + q[k + 1]}; }
 int operator()(int i) const { return b[i]; }
 Graph dag(const Graph &g) const {
  Graph ret(size());
  for (auto [s, d]: g)
   if (b[s] != b[d]) ret.add_edge(b[s], b[d]);
  return std::sort(ret.begin(), ret.end()), ret.erase(std::unique(ret.begin(), ret.end()), ret.end()), ret;
 }
};