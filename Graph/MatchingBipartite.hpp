/**
 * @title 最大マッチング(二部グラフ)
 * @category グラフ
 * @brief  O(VE)
 * @brief 速い(O(V√E)並?)
 * @brief 返り値:{マッチング数,{左の相方(いないなら-1),右の相方(いないなら-1)}}
 * @see https://snuke.hatenablog.com/entry/2019/05/07/013609
 */
// 被覆問題との関係 https://qiita.com/drken/items/7f98315b56c95a6181a4

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

struct MatchingBipartite {
  vector<vector<int>> to;
  int n, m;
  MatchingBipartite(int n, int m) : to(n), n(n), m(m) {}
  void add_edge(int l, int r) { to[l].push_back(r); }
  pair<int, pair<vector<int>, vector<int>>> get_matching() {
    vector<int> pre(n, -1), root(n, -1);
    vector<int> leftmate(n, -1), rightmate(m, -1);
    int res = 0;
    bool upd = true;
    while (upd) {
      upd = false;
      queue<int> s;
      for (int i = 0; i < n; ++i) {
        if (leftmate[i] == -1) {
          root[i] = i;
          s.push(i);
        }
      }
      while (!s.empty()) {
        int v = s.front();
        s.pop();
        if (leftmate[root[v]] != -1) continue;
        for (int i = 0; i < (int)to[v].size(); ++i) {
          int u = to[v][i];
          if (rightmate[u] == -1) {
            while (u != -1) {
              rightmate[u] = v;
              swap(leftmate[v], u);
              v = pre[v];
            }
            upd = true;
            ++res;
            break;
          }
          u = rightmate[u];
          if (pre[u] != -1) continue;
          pre[u] = v;
          root[u] = root[v];
          s.push(u);
        }
      }
      if (upd)
        fill(pre.begin(), pre.end(), -1), fill(root.begin(), root.end(), -1);
    }
    return make_pair(res, make_pair(leftmate, rightmate));
  }
};