#pragma once
#include "src/old/Tree.hpp"
class BiConnectedComponents {
 const size_t n;
 std::vector<std::pair<int, int>> E;
public:
 BiConnectedComponents(int n): n(n) {}
 void add_edge(int u, int v) { E.emplace_back(u, v); }
 Tree<void> block_cut_tree() const {
  std::vector<int> pos(n + 1), g(E.size() * 2);
  for (auto [u, v]: E) ++pos[u], ++pos[v];
  std::partial_sum(pos.begin(), pos.end(), pos.begin());
  for (auto [u, v]: E) g[--pos[u]]= v, g[--pos[v]]= u;
  std::vector<int> ord(n), par(n, -2), dat(pos.begin(), pos.begin() + n);
  int k= 0;
  for (int s= 0, p; s < n; ++s)
   if (par[s] == -2)
    for (par[p= s]= -1; p >= 0;) {
     if (dat[p] == pos[p]) ord[k++]= p;
     if (dat[p] == pos[p + 1]) {
      p= par[p];
      continue;
     }
     if (int q= g[dat[p]++]; par[q] == -2) par[q]= p, p= q;
    }
  for (int i= n; i--;) dat[ord[i]]= i;
  auto low= dat;
  for (int v= n; v--;)
   for (int j= pos[v]; j < pos[v + 1]; ++j) low[v]= std::min(low[v], dat[g[j]]);
  for (int i= n; i--;)
   if (int p= ord[i], pp= par[p]; pp >= 0) low[pp]= std::min(low[pp], low[p]);
  std::vector<std::pair<int, int>> es;
  for (int p: ord)
   if (par[p] >= 0) {
    if (int pp= par[p]; low[p] < dat[pp]) es.emplace_back(low[p]= low[pp], p);
    else es.emplace_back(k, pp), es.emplace_back(k, p), low[p]= k++;
   }
  for (int s= n; s--;)
   if (pos[s] == pos[s + 1]) es.emplace_back(k++, s);
  Tree ret(k);
  for (auto [u, v]: es) ret.add_edge(u, v);
  return ret.build(), ret;
 }
};