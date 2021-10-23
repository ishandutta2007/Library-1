#pragma once
#include <bits/stdc++.h>
/**
 * @title 接続行列の連立方程式
 * @category グラフ
 * @see https://opt-cp.com/linear-system-incidence/
 * O(V+E)
 */

// BEGIN CUT HERE

template <typename T>
class LinearSystemIncidence {
  struct Edge {
    int id, to;
    bool fwd;
  };
  int m;
  std::vector<T> x;
  std::vector<char> used;
  std::vector<std::vector<Edge>> adj;
  T dfs(std::vector<T> &b, int u) {
    used[u] = true;
    T ret = b[u];
    for (Edge e : adj[u])
      if (!used[e.to]) {
        T tmp = dfs(b, e.to);
        x[e.id] = e.fwd ? tmp : -tmp, ret += tmp;
      }
    return ret;
  }

 public:
  LinearSystemIncidence(int n) : m(0), adj(n) {}
  void add_edge(int src, int dst) {
    adj[src].emplace_back(m, dst, true), adj[dst].emplace_back(m++, src, false);
  }
  std::pair<bool, std::vector<T>> solve(std::vector<T> &b) {
    x.assign(m, T(0)), used.assign(adj.size(), false);
    for (std::size_t u = 0; u < adj.size(); u++)
      if (!used[u] && dfs(b, u) != T(0))
        return {false, std::vector<T>()};  // no sloution
    return {true, std::move(x)};
  }
};