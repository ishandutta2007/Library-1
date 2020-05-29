/**
 * @title 強連結成分分解
 * @category グラフ
 * @brief Gabow
 * @brief O(V + E)
 * @brief 返り値:{強連結成分(トポロジカルソート),ノードの属する成分の添字}
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

struct StronglyConnectedComponents {
 private:
  int n;
  vector<vector<int>> radj;

 public:
  StronglyConnectedComponents(int n) : n(n), radj(n) {}
  void add_edge(int src, int dst) { radj[dst].push_back(src); }
  pair<vector<vector<int>>, vector<int>> get_SCC() {
    vector<vector<int>> scc;
    vector<int> S, B, I(n);
    function<void(int)> dfs = [&](int u) {
      B.push_back(I[u] = S.size());
      S.push_back(u);
      for (int v : radj[u]) {
        if (!I[v])
          dfs(v);
        else
          while (I[v] < B.back()) B.pop_back();
      }
      if (I[u] == B.back()) {
        scc.push_back({});
        B.pop_back();
        for (; I[u] < S.size(); S.pop_back()) {
          scc.back().push_back(S.back());
          I[S.back()] = n + scc.size();
        }
      }
    };
    for (int u = 0; u < n; ++u)
      if (!I[u]) dfs(u);
    for (int u = 0; u < n; ++u) I[u] -= n + 1;
    return make_pair(scc, I);
  }
};
