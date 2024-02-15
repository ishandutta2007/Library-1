#pragma once
#include "src/Graph/Graph.hpp"
template <class T> std::vector<T> incidence_linear_system(const Graph &g, std::vector<T> b) {
 const int n= g.vertex_size();
 assert((int)b.size() == n);
 std::vector<T> x(g.edge_size());
 auto adje= g.adjecency_edge(0);
 std::vector<int> pre(n, -2), ei(adje.p.begin(), adje.p.begin() + n);
 for (int s= 0, p, e; s < n; ++s)
  if (pre[s] == -2)
   for (pre[p= s]= -1;;) {
    if (ei[p] == adje.p[p + 1]) {
     if (e= pre[p]; e < 0) {
      if (b[p] != T()) return {};  // no solution
      break;
     }
     T tmp= b[p];
     p= g[e].to(p);
     if constexpr (std::is_same_v<T, bool>) x[e]= tmp, b[p]= tmp ^ b[p];
     else x[e]= g[e].d == p ? -tmp : tmp, b[p]+= tmp;
    } else if (int q= g[e= adje.dat[ei[p]++]].to(p); pre[q] == -2) pre[p= q]= e;
   }
 return x;
}