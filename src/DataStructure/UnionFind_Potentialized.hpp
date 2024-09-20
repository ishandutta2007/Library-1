#pragma once
#include <vector>
#include <algorithm>
#include <cassert>
template <class weight_t> class UnionFind_Potentialized {
 std::vector<int> par;
 std::vector<weight_t> val;
public:
 UnionFind_Potentialized(int n): par(n, -1), val(n) {}
 int leader(int u) {
  if (par[u] < 0) return u;
  int r= leader(par[u]);
  if constexpr (std::is_same_v<weight_t, bool>) val[u]= val[u] ^ val[par[u]];
  else val[u]= val[par[u]] + val[u];
  return par[u]= r;
 }
 //  -p(v) + p(u) = w
 bool unite(int u, int v, weight_t w) {
  int a= leader(u), b= leader(v);
  if constexpr (std::is_same_v<weight_t, bool>) w^= val[u] ^ val[v];
  else w= val[v] + w - val[u];
  if (a == b) return w == weight_t();
  if (par[b] > par[a]) std::swap(a, b), w= -w;
  return par[b]+= par[a], par[a]= b, val[a]= w, true;
 }
 bool connected(int u, int v) { return leader(u) == leader(v); }
 int size(int u) { return -par[leader(u)]; }
 weight_t potential(int u) { return leader(u), val[u]; }
 //  -p(v) + p(u)
 weight_t diff(int u, int v) {
  if constexpr (std::is_same_v<weight_t, bool>) return potential(u) ^ potential(v);
  else return -potential(v) + potential(u);
 }
};