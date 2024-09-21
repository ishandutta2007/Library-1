#pragma once
#include <vector>
#include <algorithm>
#include <cassert>
template <class weight_t> class UnionFind_Potentialized_Undoable {
 std::vector<int> par;
 std::vector<weight_t> val;
 std::vector<std::tuple<int, int, weight_t, int>> his;
 int cur;
public:
 UnionFind_Potentialized_Undoable(int n): par(n, -1), val(n), his{{-1, -1, weight_t(), 1}}, cur(0) { his.reserve(n + 1); }
 int leader(int u) const { return par[u] < 0 ? u : leader(par[u]); }
 //  -p(v) + p(u) = w
 bool unite(int u, int v, weight_t w) {
  if constexpr (std::is_same_v<weight_t, bool>) w^= potential(v) ^ potential(u);
  else w= potential(v) + w - potential(u);
  if (++cur; (u= leader(u)) == (v= leader(v))) return ++std::get<3>(his.back()), w == weight_t();
  if (par[v] > par[u]) std::swap(u, v), w= -w;
  return his.emplace_back(u, par[u], val[u], 1), par[v]+= par[u], par[u]= v, val[u]= w, true;
 }
 bool connected(int u, int v) { return leader(u) == leader(v); }
 int size(int u) { return -par[leader(u)]; }
 weight_t potential(int u) {
  if (par[u] < 0) return val[u];
  if constexpr (std::is_same_v<weight_t, bool>) return potential(par[u]) ^ val[u];
  else return potential(par[u]) + val[u];
 }
 //  -p(v) + p(u)
 weight_t diff(int u, int v) {
  if constexpr (std::is_same_v<weight_t, bool>) return potential(u) ^ potential(v);
  else return -potential(v) + potential(u);
 }
 int time() const { return cur; }
 void undo() {
  if (assert(cur > 0), --cur; --std::get<3>(his.back()) == 0) {
   auto [u, p, v, _]= his.back();
   par[par[u]]-= p, par[u]= p, val[u]= v, his.pop_back();
  }
 }
 void rollback(int t) {
  assert(0 <= t), assert(t <= cur);
  if (t == cur) return;
  for (;;) {
   auto &[u, p, v, i]= his.back();
   if (cur-= i; cur < t) {
    i= t - cur, cur= t;
    break;
   }
   par[par[u]]-= p, par[u]= p, val[u]= v, his.pop_back();
  }
 }
};