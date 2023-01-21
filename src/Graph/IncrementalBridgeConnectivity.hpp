#pragma once
#include <unordered_set>
#include "src/DataStructure/UnionFind.hpp"
class IncrementalBridgeConnectivity {
 UnionFind cc, bcc;
 std::vector<int> bbf;
 inline int parent(int v) { return bbf[v] < 0 ? -1 : bcc.root(bbf[v]); }
 int lca(int u, int v) {
  for (std::unordered_set<int> reached;; std::swap(u, v))
   if (u != -1) {
    if (!reached.insert(u).second) return u;
    u= parent(u);
   }
 }
 void condense_path(int u, int v) {
  for (int n; !bcc.same(u, v);) n= parent(u), bbf[u]= bbf[v], bcc.unite(u, v), u= n;
 }
public:
 IncrementalBridgeConnectivity(int n): cc(n), bcc(n), bbf(n, -1) {}
 int represent(int v) { return bcc.root(v); }
 bool two_edge_connected(int u, int v) { return bcc.same(u, v); }
 bool connected(int u, int v) { return cc.same(u, v); }
 void add_edge(int u, int v) {
  if (int w; cc.same(u= bcc.root(u), v= bcc.root(v))) w= lca(u, v), condense_path(u, w), condense_path(v, w);
  else {
   if (cc.size(u) > cc.size(v)) std::swap(u, v);
   for (cc.unite(u, v); u != -1;) w= parent(u), bbf[u]= v, v= u, u= w;
  }
 }
};