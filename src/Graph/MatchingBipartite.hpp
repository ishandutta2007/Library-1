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

struct MatchingBipartite {
 private:
  std::vector<std::vector<int>> adj;
  std::vector<int> pre, rt;
  std::vector<int> lmate, rmate;

  bool dfs(int v, const int &tstamp) {
    pre[v] = tstamp;
    for (int u : adj[v]) {
      int w = rmate[u];
      if (w == -1 || (rt[w] != -2 && pre[w] != tstamp && dfs(w, tstamp))) {
        rmate[u] = v;
        lmate[v] = u;
        return true;
      }
    }
    return false;
  }

 public:
  MatchingBipartite(int n, int m)
      : adj(n), pre(n, -1), rt(n, -1), lmate(n, -1), rmate(m, -1) {}
  void add_edge(int l, int r) { adj[l].push_back(r); }
  std::pair<int, std::pair<std::vector<int>, std::vector<int>>> get_matching() {
    int res = 0;
    queue<int> que;
    for (bool update = true; update;) {
      update = false;
      for (int i = 0; i < (int)adj.size(); i++)
        if (lmate[i] == -1) que.push(rt[i] = i);
      while (!que.empty()) {
        int v = que.front();
        que.pop();
        if (lmate[rt[v]] != -1) continue;
        for (int u : adj[v]) {
          if (rmate[u] == -1) {
            for (; u != -1; v = pre[v]) rmate[u] = v, swap(lmate[v], u);
            update = true;
            res++;
            break;
          }
          u = rmate[u];
          if (pre[u] != -1) continue;
          rt[u] = rt[pre[u] = v];
          que.push(u);
        }
      }
      if (update)
        std::fill(pre.begin(), pre.end(), -1), fill(rt.begin(), rt.end(), -1);
    }
    return std::make_pair(res, std::make_pair(lmate, rmate));
  }
  std::pair<int, std::pair<std::vector<int>, std::vector<int>>>
  lexicographically_matching() {
    int res = get_matching().first;
    int tstamp = -2;
    for (int i = 0; i < (int)adj.size(); i++) {
      if (lmate[i] != -1) {
        rmate[lmate[i]] = -1;
        lmate[i] = -1;
        dfs(i, --tstamp);
        rt[i] = -2;
      }
    }
    return std::make_pair(res, std::make_pair(lmate, rmate));
  }
};
