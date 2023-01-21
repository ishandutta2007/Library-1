#pragma once
#include <vector>
#include <algorithm>
class UnionFind {
 std::vector<int> par;
public:
 UnionFind(int n): par(n, -1) {}
 bool unite(int u, int v) {
  if ((u= root(u)) == (v= root(v))) return false;
  if (par[u] > par[v]) std::swap(u, v);
  return par[u]+= par[v], par[v]= u, true;
 }
 bool same(int u, int v) { return root(u) == root(v); }
 int root(int u) { return par[u] < 0 ? u : par[u]= root(par[u]); }
 int size(int u) { return -par[root(u)]; }
};