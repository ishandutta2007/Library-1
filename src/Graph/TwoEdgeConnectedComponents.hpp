#pragma once
#include <bits/stdc++.h>
/**
 * @title 二重辺連結成分分解
 * @category グラフ
 *  O(V + E)
 *  返り値:{二重辺連結成分,ノードの属する成分の添字}
 */

// BEGIN CUT HERE

struct TwoEdgeConnectedComponents {
 private:
  int n;
  std::vector<std::vector<int>> adj;

 public:
  TwoEdgeConnectedComponents(int n) : n(n), adj(n) {}
  void add_edge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
  std::pair<std::vector<std::vector<int>>, std::vector<int>> get_2ECC() {
    std::vector<int> index(n, -1), num(n), par(n, -1), cur(n);
    std::vector<short> parcheck(n, 0);
    std::vector<std::vector<int>> block;
    for (int s = 0; s < n; ++s) {
      if (num[s]) continue;
      int time = 0;
      std::vector<int> snum, path, stack = {s};
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
    return std::make_pair(block, index);
  }
};
