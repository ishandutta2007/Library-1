#pragma once
#include "src/DataStructure/PersistentArray.hpp"
struct UnionFind_Persistent {
 PersistentArray<int, 64> par;
 UnionFind_Persistent() {}
 UnionFind_Persistent(int n): par(n, -1) {}
 bool unite(int u, int v) {
  if ((u= root(u)) == (v= root(v))) return false;
  if (par.get(u) > par.get(v)) std::swap(u, v);
  par[u]+= par.get(v), par[v]= u;
  return true;
 }
 bool same(int u, int v) { return root(u) == root(v); }
 int root(int u) { return par.get(u) < 0 ? u : root(par.get(u)); }
 int size(int u) { return -par.get(root(u)); }
};
