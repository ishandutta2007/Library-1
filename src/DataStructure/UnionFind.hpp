/**
 * @title Union-Find
 * @category データ構造
 * @brief unite(x,y) x が入っている集合と y が入っている集合を併合する．
 * @brief same(x,y) x と y が同じ集合に入っているかどうかを判定する．
 * @brief O(α(N))
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

struct UnionFind {
  vector<int> par;
  UnionFind(int size) : par(size, -1) {}
  bool unite(int u, int v) {
    if ((u = root(u)) == (v = root(v))) return false;
    if (par[u] > par[v]) swap(u, v);
    par[u] += par[v], par[v] = u;
    return true;
  }
  bool same(int u, int v) { return root(u) == root(v); }
  int root(int u) { return par[u] < 0 ? u : par[u] = root(par[u]); }
  int size(int u) { return -par[root(u)]; }
};