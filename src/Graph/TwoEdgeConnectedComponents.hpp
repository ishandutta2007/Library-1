/**
 * @title 二重辺連結成分分解
 * @category グラフ
 * @brief O(V + E)
 * @brief 返り値:{二重辺連結成分,ノードの属する成分の添字}
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

struct TwoEdgeConnectedComponents {
 private:
  int n;
  vector<vector<int>> adj;

 public:
  TwoEdgeConnectedComponents(int n) : n(n), adj(n) {}
  void add_edge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
  pair<vector<vector<int>>, vector<int>> get_2ECC() {
    vector<int> index(n, -1), num(n), par(n, -1), cur(n);
    vector<short> parcheck(n, 0);
    vector<vector<int>> block;
    for (int s = 0; s < n; ++s) {
      if (num[s]) continue;
      int time = 0;
      vector<int> snum, path, stack = {s};
      while (!stack.empty()) {
        int u = stack.back();
        if (cur[u] == 0) {
          num[u] = ++time;
          path.push_back(u);
          snum.push_back(num[u]);
        }
        if (cur[u] == adj[u].size()) {
          if (num[u] == snum.back()) {
            snum.pop_back();
            block.push_back({});
            while (1) {
              int w = path.back();
              path.pop_back();
              block.back().push_back(w);
              index[w] = block.size() - 1;
              if (u == w) break;
            }
          }
          stack.pop_back();
        } else {
          int v = adj[u][cur[u]++];
          if (!num[v]) {
            par[v] = u;
            stack.push_back(v);
          } else if (v == par[u] && !parcheck[u]) {
            parcheck[u] = true;
          } else if (index[v] < 0) {
            while (snum.back() > num[v]) snum.pop_back();
          }
        }
      }
    }
    return make_pair(block, index);
  }
};
