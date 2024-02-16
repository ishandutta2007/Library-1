#pragma once
#include <vector>
#include <algorithm>
template <bool undoable= false> class UnionFind {
 std::vector<int> par;
 std::vector<std::pair<int, int>> his;
public:
 UnionFind(int n): par(n, -1) {}
 bool unite(int u, int v) {
  if ((u= root(u)) == (v= root(v))) return false;
  if (par[u] > par[v]) std::swap(u, v);
  if constexpr (undoable) his.emplace_back(v, par[v]);
  return par[u]+= par[v], par[v]= u, true;
 }
 bool same(int u, int v) { return root(u) == root(v); }
 int root(int u) {
  if constexpr (undoable) return par[u] < 0 ? u : root(par[u]);
  else return par[u] < 0 ? u : par[u]= root(par[u]);
 }
 int size(int u) { return -par[root(u)]; }
 int time() const {
  static_assert(undoable, "\'time\' is not enabled");
  return his.size();
 }
 void undo() {
  static_assert(undoable, "\'undo\' is not enabled");
  auto [u, s]= his.back();
  assert(par[par[u]] < 0);
  his.pop_back(), par[par[u]]-= s, par[u]= s;
 }
 void rollback(size_t t) {
  static_assert(undoable, "\'rollback\' is not enabled");
  assert(t <= his.size());
  while (his.size() > t) undo();
 }
};