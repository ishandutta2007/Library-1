#pragma once
#include <bits/stdc++.h>
/**
 * @title 最小カット問題のk値への一般化
 * @category アルゴリズム
 * @see https://noshi91.hatenablog.com/entry/2021/06/29/044225
 * phi関数等で ∞ を返すときはその大きさに注意 (大きすぎるとオーバーフロー)
 */

// verify用: https://atcoder.jp/contests/kupc2019/tasks/kupc2019_h

// BEGIN CUT HERE

template <typename MF, typename Th, typename Ph>
auto monge_mincut(int n, int k, Th theta, Ph phi) {
  using T = typename MF::flow_t;
  static constexpr T INF = std::numeric_limits<T>::max();
  T ret = 0;
  MF graph;
  int s = graph.add_vertex(), t = graph.add_vertex();
  std::vector<std::vector<int>> x;
  std::vector<std::vector<T>> th(n, std::vector<T>(k));
  for (int i = 0; i < n; i++) {
    x.emplace_back(graph.add_vertices(k - 1));
    for (int l = 1; l < k - 1; l++) graph.add_edge(x[i][l], x[i][l - 1], INF);
    for (int l = 0; l < k; l++) th[i][l] = theta(i, l);
  }
  for (int i = 0; i < n; i++)
    for (int j = i + 1; j < n; j++) {
      for (int l = 0; l < k - 1; l++)
        for (int m = 0; m < k - 1; m++) {
          T cost = -phi(i, j, l + 1, m + 1) + phi(i, j, l, m + 1) +
                   phi(i, j, l + 1, m) - phi(i, j, l, m);
          assert(cost >= 0);  // monge
          if (cost > 0) graph.add_edge(x[i][l], x[j][m], cost);
        }
      for (int l = 0; l < k; l++) th[i][l] += phi(i, j, l, k - 1);
      for (int l = 0; l < k; l++) th[j][l] += phi(i, j, 0, l);
      ret -= phi(i, j, 0, k - 1);
    }
  for (int i = 0; i < n; i++) {
    ret += th[i][0];
    for (int l = 0; l < k - 1; l++) {
      T cost = th[i][l] - th[i][l + 1];
      if (cost > 0) graph.add_edge(s, x[i][l], cost), ret -= cost;
      if (cost < 0) graph.add_edge(x[i][l], t, -cost);
    }
  }
  ret += graph.maxflow(s, t);
  auto y = graph.mincut(s);
  std::vector<int> sol(n, k - 1);
  for (int i = 0; i < n; i++)
    for (int l = 0; l < k - 1; l++)
      if (!y[x[i][l]]) sol[i] = l, l = k;
  return std::make_pair(ret, sol);
}