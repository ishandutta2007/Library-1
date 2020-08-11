/**
 * @title Union-Find(ポテンシャル)
 * @category データ構造
 * @brief 各ノードにポテンシャルをもたせ、その差を求められる
 * @brief O(α(N))
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

template <typename Weight>
struct UnionFind_Potential {
  vector<int> par;
  vector<Weight> val;
  UnionFind_Potential(int size) : par(size, -1), val(size, 0) {}
  bool unite(int v, int u, Weight w) {
    w += potential(u) - potential(v);
    if ((u = root(u)) == (v = root(v))) return false;
    if (par[u] > par[v]) swap(u, v), w = -w;
    par[u] += par[v], par[v] = u, val[v] = w;
    return true;
  }
  bool same(int u, int v) { return root(u) == root(v); }
  int root(int u) {
    if (par[u] < 0) return u;
    int r = root(par[u]);
    val[u] += val[par[u]];
    return par[u] = r;
  }
  int size(int u) { return -par[root(u)]; }
  Weight potential(int u) {
    root(u);
    return val[u];
  }
  Weight diff(int u, int v) { return potential(u) - potential(v); }
};