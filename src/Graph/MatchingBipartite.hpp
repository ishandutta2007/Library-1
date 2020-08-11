/**
 * @title 最大マッチング(二部グラフ)
 * @category グラフ
 *   O(VE)
 *  速い(O(E√V)並?)
 *  返り値:{マッチング数,{左の相方(いないなら-1),右の相方(いないなら-1)}}
 * @see https://snuke.hatenablog.com/entry/2019/05/07/013609
 */
// 被覆問題との関係 https://qiita.com/drken/items/7f98315b56c95a6181a4

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

struct MatchingBipartite {
  vector<vector<int>> adj;
  int n, m;
  MatchingBipartite(int n, int m) : adj(n), n(n), m(m) {}
  void add_edge(int l, int r) { adj[l].push_back(r); }
  pair<int, pair<vector<int>, vector<int>>> get_matching() {
    vector<int> pre(n, -1), root(n, -1);
    vector<int> leftmate(n, -1), rightmate(m, -1);
    int res = 0;
    for (bool update = true; update;) {
      update = false;
      queue<int> que;
      for (int i = 0; i < n; ++i)
        if (leftmate[i] == -1) que.push(root[i] = i);
      while (!que.empty()) {
        int v = que.front();
        que.pop();
        if (leftmate[root[v]] != -1) continue;
        for (int u : adj[v]) {
          if (rightmate[u] == -1) {
            for (; u != -1; v = pre[v]) rightmate[u] = v, swap(leftmate[v], u);
            update = true;
            res++;
            break;
          }
          u = rightmate[u];
          if (pre[u] != -1) continue;
          root[u] = root[pre[u] = v];
          que.push(u);
        }
      }
      if (update)
        fill(pre.begin(), pre.end(), -1), fill(root.begin(), root.end(), -1);
    }
    return make_pair(res, make_pair(leftmate, rightmate));
  }
};