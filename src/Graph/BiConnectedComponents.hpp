#pragma once
#include <vector>
#include <algorithm>
class BiConnectedComponents {
 std::vector<std::vector<int>> adj;
public:
 BiConnectedComponents(int n): adj(n) {}
 void add_edge(int u, int v) { adj[u].push_back(v), adj[v].push_back(u); }
 std::vector<std::vector<int>> block_cut_tree() const {
  const int n= adj.size();
  std::vector<int> ord(n), par(n, -2), dat(n, 0), low;
  std::vector<std::vector<int>> ret(n);
  auto add= [&](int u, int v) { ret[u].push_back(v), ret[v].push_back(u); };
  int k= 0;
  for (int s= 0, p; s < n; ++s)
   if (par[s] == -2)
    for (par[p= s]= -1; p >= 0;) {
     if (dat[p] == 0) ord[k++]= p;
     if (dat[p] == (int)adj[p].size()) {
      p= par[p];
      continue;
     }
     if (int q= adj[p][dat[p]++]; par[q] == -2) par[q]= p, p= q;
    }
  for (int i= 0; i < n; ++i) dat[ord[i]]= i;
  low= dat;
  for (int v= 0; v < n; ++v)
   for (int u: adj[v]) low[v]= std::min(low[v], dat[u]);
  for (int i= n; i--;)
   if (int p= ord[i], pp= par[p]; pp >= 0) low[pp]= std::min(low[pp], low[p]);
  for (int p: ord)
   if (par[p] >= 0) {
    if (int pp= par[p]; low[p] < dat[pp]) low[p]= low[pp], add(low[p], p);
    else ret.resize(k + 1), add(k, pp), add(k, p), low[p]= k++;
   }
  for (int s= 0; s < n; ++s)
   if (!adj[s].size()) ret.resize(k + 1), add(k++, s);
  return ret;
 }
};