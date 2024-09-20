#pragma once
#include <vector>
#include <algorithm>
#include <array>
#include <cassert>
class UnionFind_Undoable {
 std::vector<int> par;
 std::vector<std::array<int, 3>> his;
 int cur;
public:
 UnionFind_Undoable(int n): par(n, -1), his{{-1, -1, 1}}, cur(0) { his.reserve(n + 1); }
 int leader(int u) const { return par[u] < 0 ? u : leader(par[u]); }
 bool unite(int u, int v) {
  if (++cur; (u= leader(u)) == (v= leader(v))) return ++his.back()[2], false;
  if (par[u] > par[v]) std::swap(u, v);
  return his.push_back({v, par[v], 1}), par[u]+= par[v], par[v]= u, true;
 }
 bool connected(int u, int v) const { return leader(u) == leader(v); }
 int size(int u) const { return -par[leader(u)]; }
 int time() const { return cur; }
 void undo() {
  if (assert(cur > 0), --cur; --his.back()[2] == 0) {
   auto [v, p, _]= his.back();
   his.pop_back(), par[par[v]]-= p, par[v]= p;
  }
 }
 void rollback(int t) {
  assert(0 <= t), assert(t <= cur);
  if (t == cur) return;
  for (;;) {
   auto &[u, p, i]= his.back();
   if (cur-= i; cur < t) {
    i= t - cur, cur= t;
    break;
   }
   par[par[u]]-= p, par[u]= p, his.pop_back();
  }
 }
};