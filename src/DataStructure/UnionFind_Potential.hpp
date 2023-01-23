#pragma once
#include <vector>
#include <algorithm>
template <typename Weight> struct UnionFind_Potential {
 std::vector<int> par;
 std::vector<Weight> val;
 UnionFind_Potential(int size): par(size, -1), val(size, 0) {}
 bool unite(int v, int u, Weight w) {
  if (w+= potential(u) - potential(v); (u= root(u)) == (v= root(v))) return false;
  if (par[u] > par[v]) std::swap(u, v), w= -w;
  return par[u]+= par[v], par[v]= u, val[v]= w, true;
 }
 bool same(int u, int v) { return root(u) == root(v); }
 int root(int u) {
  if (par[u] < 0) return u;
  int r= root(par[u]);
  return val[u]+= val[par[u]], par[u]= r;
 }
 int size(int u) { return -par[root(u)]; }
 Weight potential(int u) { return root(u), val[u]; }
 Weight diff(int u, int v) { return potential(u) - potential(v); }
};