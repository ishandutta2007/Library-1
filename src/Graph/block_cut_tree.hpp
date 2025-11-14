#pragma once
#include "src/Graph/Graph.hpp"
// [0,n) : vertex
// [n,n+b) : block
// deg(v) > 1 -> articulation point
Graph block_cut_tree(const CSRArray<int> &adj) {
 const int n= adj.size();
 std::vector<int> ord(n), par(n, -2), dat(adj.p.begin(), adj.p.begin() + n);
 int k= 0;
 for (int s= n, p; s--;)
  if (par[s] == -2)
   for (par[p= s]= -1; p >= 0;) {
    if (dat[p] == adj.p[p]) ord[k++]= p;
    if (dat[p] == adj.p[p + 1]) p= par[p];
    else if (int q= adj.dat[dat[p]++]; par[q] == -2) par[q]= p, p= q;
   }
 for (int i= n; i--;) dat[ord[i]]= i;
 auto low= dat;
 for (int v= n; v--;)
  for (int u: adj[v]) low[v]= std::min(low[v], dat[u]);
 for (int i= n; i--;)
  if (int p= ord[i], pp= par[p]; pp >= 0) low[pp]= std::min(low[pp], low[p]);
 Graph ret(k);
 for (int p: ord)
  if (par[p] >= 0) {
   if (int pp= par[p]; low[p] < dat[pp]) ret.add_edge(low[p]= low[pp], p);
   else ret.add_vertex(), ret.add_edge(k, pp), ret.add_edge(k, p), low[p]= k++;
  }
 for (int s= 0; s < n; ++s)
  if (!adj[s].size()) ret.add_edge(ret.add_vertex(), s);
 return ret;
}
Graph block_cut_tree(const Graph &g) { return block_cut_tree(g.adjacency_vertex(0)); }