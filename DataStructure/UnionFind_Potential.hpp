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

struct UnionFind_Potential {
  typedef long long Weight;
  vector<int> par;
  vector<Weight> val;
  UnionFind_Potential(int size) : par(size, -1), val(size, 0) {}
  bool unionSet(int y, int x, Weight w) {
    w += potential(x) - potential(y);
    x = root(x), y = root(y);
    if (x != y) {
      if (par[y] < par[x]) swap(x, y), w = -w;
      par[x] += par[y];
      par[y] = x, val[y] = w;
    }
    return x != y;
  }
  bool same(int x, int y) { return root(x) == root(y); }
  int root(int x) {
    if (par[x] < 0) return x;
    int r = root(par[x]);
    val[x] += val[par[x]];
    return par[x] = r;
  }
  int size(int x) { return -par[root(x)]; }
  Weight potential(int x) {
    root(x);
    return val[x];
  }
  Weight diff(int x, int y) { return potential(x) - potential(y); }
};