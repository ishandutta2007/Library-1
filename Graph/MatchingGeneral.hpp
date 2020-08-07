/**
 * @title 最大マッチング(一般グラフ)
 * @category グラフ
 * @brief  O(VE log V)
 * @brief GabowのEdmonds' Algorithm
 * @brief 返り値:{マッチング数,各頂点の相方(いないなら-1）}
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

struct MatchingGeneral {
 private:
  int n, res;
  vector<vector<int>> adj;
  vector<int> mate, idx, p;
  vector<pair<int, int>> edges;

 private:
  void rematch(int u, int v) {
    int w = mate[u];
    mate[u] = v;
    if (w == -1 || mate[w] != u) return;
    if (edges[u].second == -1) {
      mate[w] = edges[u].first;
      rematch(edges[u].first, w);
    } else {
      rematch(edges[u].first, edges[u].second);
      rematch(edges[u].second, edges[u].first);
    }
  }
  bool check(int root) {
    function<int(int)> f = [&](int x) {
      return (idx[x] != res || p[x] == -1) ? x : (p[x] = f(p[x]));
    };
    queue<int> que;
    que.push(root);
    p[root] = -1;
    idx[root] = res;
    edges[root] = {-1, -1};
    function<void(int, int)> g = [&](int t, int w) {
      while (t != w) {
        idx[t] = res;
        p[t] = w;
        que.push(t);
        t = f(edges[mate[t]].first);
      }
    };
    while (!que.empty()) {
      int x = que.front();
      que.pop();
      for (int y : adj[x])
        if (y != root) {
          if (mate[y] == -1) {
            mate[y] = x;
            rematch(x, y);
            return true;
          } else if (idx[y] == res) {
            int u = f(x), v = f(y), w = root;
            if (u == v) continue;
            while (u != root || v != root) {
              if (v != root) swap(u, v);
              if (edges[u].first == x && edges[u].second == y) {
                w = u;
                break;
              }
              edges[u] = {x, y};
              u = f(edges[mate[u]].first);
            }
            g(f(x), w);
            g(f(y), w);
          } else if (idx[mate[y]] != res) {
            edges[y] = {-1, -1};
            idx[mate[y]] = res;
            p[mate[y]] = y;
            edges[mate[y]] = {x, -1};
            que.push(mate[y]);
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
  pair<int, vector<int>> get_matching() {
    for (int i = 0; i < n; i++)
      if (mate[i] == -1) res += check(i);
    return make_pair(res, mate);
  }
};
