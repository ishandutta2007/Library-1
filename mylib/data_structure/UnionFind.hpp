#pragma once
#include <vector>
#include <algorithm>
class UnionFind {
 std::vector<int> par;
public:
 UnionFind(int n): par(n, -1) {}
 int leader(int u) { return par[u] < 0 ? u : par[u]= leader(par[u]); }
 bool unite(int u, int v) {
  if ((u= leader(u)) == (v= leader(v))) return false;
  if (par[u] > par[v]) std::swap(u, v);
  return par[u]+= par[v], par[v]= u, true;
 }
 bool connected(int u, int v) { return leader(u) == leader(v); }
 int size(int u) { return -par[leader(u)]; }
};