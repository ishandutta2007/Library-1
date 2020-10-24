#pragma once
#include <bits/stdc++.h>
/**
 * @title 最大マッチング(一般グラフ)
 * @category グラフ
 *   O(VE log V)
 *  GabowのEdmonds' Algorithm
 *  返り値:{マッチング数,各頂点の相方(いないなら-1）}
 */

// BEGIN CUT HERE

struct MatchingGeneral {
 private:
  int n, res;
  std::vector<std::vector<int>> adj;
  std::vector<int> mate, idx, p;
  std::vector<std::pair<int, int>> edges;

 private:
  void rematch(int u, int v) {
    int w = mate[u];
    mate[u] = v;
    if (w == -1 || mate[w] != u) return;
    if (edges[u].second == -1) {
      rematch(mate[w] = edges[u].first, w);
    } else {
      rematch(edges[u].first, edges[u].second);
      rematch(edges[u].second, edges[u].first);
    }
  }
  bool check(int root) {
    std::function<int(int)> f = [&](int x) {
      return (idx[x] != res || p[x] == -1) ? x : (p[x] = f(p[x]));
    };
    std::queue<int> que;
    edges[root] = {-1, -1};
    idx[root] = res, p[root] = -1, que.push(root);
    while (!que.empty()) {
      int x = que.front();
      que.pop();
      for (int y : adj[x])
        if (y != root) {
          if (mate[y] == -1) {
            rematch(mate[y] = x, y);
            return true;
          } else if (idx[y] == res) {
            int u = f(x), v = f(y), w = root;
            if (u == v) continue;
            while (u != root || v != root) {
              if (v != root) std::swap(u, v);
              if (edges[u].first == x && edges[u].second == y) {
                w = u;
                break;
              }
              edges[u] = {x, y};
              u = f(edges[mate[u]].first);
            }
            for (int t = f(x); t != w; t = f(edges[mate[t]].first))
              idx[t] = res, p[t] = w, que.push(t);
            for (int t = f(y); t != w; t = f(edges[mate[t]].first))
              idx[t] = res, p[t] = w, que.push(t);
          } else if (idx[mate[y]] != res) {
            edges[y] = {-1, -1};
            edges[mate[y]] = {x, -1};
            idx[mate[y]] = res, p[mate[y]] = y, que.push(mate[y]);
          }
        }
    }
    return false;
  }

 public:
  MatchingGeneral(int n)
      : n(n), res(0), adj(n), mate(n, -1), idx(n, -1), p(n), edges(n) {}
  void add_edge(int u, int v) {
    adj[u].emplace_back(v);
    adj[v].emplace_back(u);
  }
  std::pair<int, std::vector<int>> get_matching() {
    for (int i = 0; i < n; i++)
      if (mate[i] == -1) res += check(i);
    return std::make_pair(res, mate);
  }
};