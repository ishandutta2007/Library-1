#pragma once
#include <vector>
#include <array>
#include <algorithm>
class StronglyConnectedComponents {
 std::vector<std::vector<int>> adj, rev;
public:
 StronglyConnectedComponents(int n): adj(n), rev(n) {}
 void add_edge(int src, int dst) { adj[src].push_back(dst), rev[dst].push_back(src); }
 std::vector<std::vector<int>> get_block() const {
  const int n= adj.size();
  std::vector<std::vector<int>> blk;
  std::vector<int> ord(n), par(n, -2), dat(n, 0);
  int k= n;
  for (int s= 0, p; s < n; ++s)
   if (par[s] == -2)
    for (par[p= s]= -1; p >= 0;) {
     if (dat[p] == (int)adj[p].size()) {
      ord[--k]= p, p= par[p];
      continue;
     }
     if (int q= adj[p][dat[p]++]; par[q] == -2) par[q]= p, p= q;
    }
  dat.assign(n, 1);
  for (int s: ord)
   if (dat[s]) {
    blk.resize(++k), dat[s]= 0, blk.back().push_back(s);
    for (int i= 0; i < (int)blk.back().size(); ++i)
     for (int v: rev[blk.back()[i]])
      if (dat[v]) dat[v]= 0, blk.back().push_back(v);
   }
  return blk;
 }
 std::vector<int> get_index(const std::vector<std::vector<int>> &blk) const {
  std::vector<int> index(adj.size());
  for (int i= blk.size(); i--;)
   for (int v: blk[i]) index[v]= i;
  return index;
 }
 std::vector<std::vector<int>> get_dag(const std::vector<int> &index, int num) const {
  std::vector<std::vector<int>> dag(num);
  std::vector<std::array<int, 2>> es;
  for (int i= adj.size(); i--;)
   for (int j: adj[i])
    if (int u= index[i], v= index[j]; u != v) es.push_back({u, v});
  std::sort(es.begin(), es.end()), es.erase(std::unique(es.begin(), es.end()), es.end());
  for (auto [u, v]: es) dag[u].push_back(v);
  return dag;
 }
};