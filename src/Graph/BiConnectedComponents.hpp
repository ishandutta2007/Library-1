#pragma once
#include <vector>
#include <algorithm>
class BiConnectedComponents {
 std::vector<std::vector<int>> adj;
public:
 BiConnectedComponents(int n): adj(n) {}
 void add_edge(int u, int v) { adj[u].push_back(v), adj[v].push_back(u); }
 std::vector<std::vector<int>> block_cut_tree() {
  int n= adj.size(), m= n, k= 0;
  std::vector<int> low(n), ord(n), st;
  std::vector<bool> used(n);
  std::vector<std::vector<int>> bct;
  st.reserve(n);
  auto dfs= [&](auto &self, int v, int p) -> void {
   int ch= 0, s, x;
   st.push_back(v), used[v]= true, low[v]= ord[v]= k++;
   for (int u: adj[v])
    if (u != p) {
     if (used[u]) low[v]= std::min(low[v], ord[u]);
     else if (s= st.size(), ++ch, self(self, u, v), low[v]= std::min(low[v], low[u]); (p == -1 && ch > 1) || (p != -1 && low[u] >= ord[v])) {
      for (bct.resize(m + 1), bct[m].push_back(v), bct[v].push_back(m); st.size() > s; st.pop_back()) bct[x= st.back()].push_back(m), bct[m].push_back(x);
      ++m;
     }
    }
  };
  for (int v= n; v--;)
   if (!used[v]) {
    dfs(dfs, v, -1), bct.resize(m + 1);
    for (int x: st) bct[x].push_back(m), bct[m].push_back(x);
    ++m, st.clear();
   }
  return bct;
 }
};