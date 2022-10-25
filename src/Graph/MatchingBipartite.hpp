#pragma once
#include <bits/stdc++.h>
/**
 * @title 最大マッチング(二部グラフ)
 * @category グラフ
 *  O(VE) 速い(O(E√V)並?)
 *  返り値:{マッチング数,{左の相方(いないなら-1),右の相方(いないなら-1)}}
 *  lexicographically_matching 辞書順最小
 * @see https://snuke.hatenablog.com/entry/2019/05/07/013609
 */
// 被覆問題との関係 https://qiita.com/drken/items/7f98315b56c95a6181a4

// BEGIN CUT HERE

class MatchingBipartite {
  std::vector<std::vector<int>> adj;
  std::vector<int> pre, rt, lmate, rmate;
  bool dfs(int v, const int &tstamp) {
    pre[v] = tstamp;
    for (int u : adj[v])
      if (int w = rmate[u];
          w == -1 || (rt[w] != -2 && pre[w] != tstamp && dfs(w, tstamp)))
        return rmate[u] = v, lmate[v] = u, true;
    return false;
  }

 public:
  MatchingBipartite(int n, int m) : adj(n), lmate(n, -1), rmate(m, -1) {}
  void add_edge(int l, int r) { adj[l].push_back(r); }
  std::pair<int, std::pair<std::vector<int>, std::vector<int>>> get_matching() {
    int res = 0;
    std::queue<int> que;
    for (bool update = true; update;) {
      update = false, pre.assign(adj.size(), -1), rt.assign(adj.size(), -1);
      for (int i = 0; i < (int)adj.size(); i++)
        if (lmate[i] == -1) que.push(rt[i] = i);
      while (!que.empty()) {
        int v = que.front();
        if (que.pop(); lmate[rt[v]] != -1) continue;
        for (int u : adj[v]) {
          if (rmate[u] == -1) {
            for (; u != -1; v = pre[v]) rmate[u] = v, std::swap(lmate[v], u);
            update = true, res++;
            break;
          }
          if (pre[rmate[u]] != -1) continue;
          rt[rmate[u]] = rt[pre[rmate[u]] = v], que.push(rmate[u]);
        }
      }
    }
    return std::make_pair(res, std::make_pair(lmate, rmate));
  }
  std::pair<int, std::pair<std::vector<int>, std::vector<int>>>
  lexicographically_matching() {
    int res = get_matching().first, tstamp = -2;
    for (int i = 0; i < (int)adj.size(); i++)
      if (lmate[i] != -1)
        lmate[i] = rmate[lmate[i]] = -1, dfs(i, --tstamp), rt[i] = -2;
    return std::make_pair(res, std::make_pair(lmate, rmate));
  }
};