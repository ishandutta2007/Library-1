#pragma once
#include <bits/stdc++.h>
#include "src/DataStructure/PersistentArray.hpp"
/**
 * @title Union-Find(完全永続)
 * @category データ構造
 * 経路圧縮なし O(logN)
 */
// verify用:
// https://atcoder.jp/contests/code-thanks-festival-2017/tasks/code_thanks_festival_2017_h

// BEGIN CUT HERE

struct UnionFind_Persistent {
  PersistentArray<int> par;
  UnionFind_Persistent() {}
  UnionFind_Persistent(int n) : par(n, -1) {}
  bool unite(int u, int v) {
    if ((u = root(u)) == (v = root(v))) return false;
    if (par.get(u) > par.get(v)) std::swap(u, v);
    par[u] += par.get(v), par[v] = u;
    return true;
  }
  bool same(int u, int v) { return root(u) == root(v); }
  int root(int u) { return par.get(u) < 0 ? u : root(par.get(u)); }
  int size(int u) { return -par.get(root(u)); }
};
