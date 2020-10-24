#pragma once
#include <bits/stdc++.h>
/**
 * @title 強連結成分分解
 * @category グラフ
 *  Gabow
 *  O(V + E)
 *  返り値:{強連結成分(トポロジカルソート),ノードの属する成分の添字}
 */

// BEGIN CUT HERE

struct StronglyConnectedComponents {
  std::vector<std::vector<int>> adj;

  StronglyConnectedComponents(int n) : adj(n) {}
  void add_edge(int src, int dst) { adj[src].push_back(dst); }
  std::pair<std::vector<std::vector<int>>, std::vector<int>> get_SCC() {
    std::vector<std::vector<int>> scc;
    std::vector<int> S, B, index(adj.size());
    std::function<void(int)> dfs = [&](int u) {
      B.push_back(index[u] = S.size());
      S.push_back(u);
      for (int v : adj[u]) {
        if (!index[v])
          dfs(v);
        else
          while (index[v] < B.back()) B.pop_back();
      }
      if (index[u] == B.back()) {
        scc.push_back({});
        B.pop_back();
        for (; index[u] < (int)S.size(); S.pop_back()) {
          scc.back().push_back(S.back());
          index[S.back()] = adj.size() + scc.size();
        }
      }
    };
    for (std::size_t u = 0; u < adj.size(); u++)
      if (!index[u]) dfs(u);
    reverse(scc.begin(), scc.end());
    for (std::size_t u = 0; u < adj.size(); u++)
      index[u] = scc.size() - index[u] + adj.size();
    return std::make_pair(scc, index);
  }
};
