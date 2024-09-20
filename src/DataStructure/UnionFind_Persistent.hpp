#pragma once
#include "src/DataStructure/PersistentArray.hpp"
struct UnionFind_Persistent {
 PersistentArray<int, 64> par;
 UnionFind_Persistent() {}
 UnionFind_Persistent(int n): par(n, -1) {}
 bool unite(int u, int v) {
  if ((u= leader(u)) == (v= leader(v))) return false;
  if (par.get(u) > par.get(v)) std::swap(u, v);
  par[u]+= par.get(v), par[v]= u;
  return true;
 }
 bool connected(int u, int v) const { return leader(u) == leader(v); }
 int leader(int u) const { return par.get(u) < 0 ? u : leader(par.get(u)); }
 int size(int u) const { return -par.get(leader(u)); }
};
