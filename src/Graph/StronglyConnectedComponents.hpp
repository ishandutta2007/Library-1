/**
 * @title 強連結成分分解
 * @category グラフ
 *  Gabow
 *  O(V + E)
 *  返り値:{強連結成分(トポロジカルソート),ノードの属する成分の添字}
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

struct StronglyConnectedComponents {
  vector<vector<int>> adj;

  StronglyConnectedComponents(int n) : adj(n) {}
  void add_edge(int src, int dst) { adj[src].push_back(dst); }
  pair<vector<vector<int>>, vector<int>> get_SCC() {
    vector<vector<int>> scc;
    vector<int> S, B, index(adj.size());
    function<void(int)> dfs = [&](int u) {
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
    for (size_t u = 0; u < adj.size(); u++)
      if (!index[u]) dfs(u);
    reverse(scc.begin(), scc.end());
    for (size_t u = 0; u < adj.size(); u++)
      index[u] = scc.size() - index[u] + adj.size();
    return make_pair(scc, index);
  }
};
