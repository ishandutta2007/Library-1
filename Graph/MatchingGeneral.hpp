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
  struct edge {
    int u, v;
  };

 private:
  int n, res;
  vector<vector<int>> g;
  vector<int> mate, idx, p;
  vector<edge> es;

 private:
  void rematch(int u, int v) {
    int w = mate[u];
    mate[u] = v;
    if (w == -1 || mate[w] != u) return;
    if (es[u].v == -1) {
      mate[w] = es[u].u;
      rematch(es[u].u, w);
    } else {
      rematch(es[u].u, es[u].v);
      rematch(es[u].v, es[u].u);
    }
  }
  bool check(int rt) {
    function<int(int)> f = [&](int x) {
      return (idx[x] != res || p[x] == -1) ? x : (p[x] = f(p[x]));
    };
    queue<int> que;
    que.push(rt);
    p[rt] = -1;
    idx[rt] = res;
    es[rt] = {-1, -1};
    while (!que.empty()) {
      int x = que.front();
      que.pop();
      for (int y : g[x])
        if (y != rt) {
          if (mate[y] == -1) {
            mate[y] = x;
            rematch(x, y);
            return true;
          } else if (idx[y] == res) {
            int u = f(x), v = f(y), w = rt;
            if (u == v) continue;
            while (u != rt || v != rt) {
              if (v != rt) swap(u, v);
              if (es[u].u == x && es[u].v == y) {
                w = u;
                break;
              }
              es[u] = {x, y};
              u = f(es[mate[u]].u);
            }
            int t = f(x);
            while (t != w) {
              idx[t] = res;
              p[t] = w;
              que.push(t);
              t = f(es[mate[t]].u);
            }
            t = f(y);
            while (t != w) {
              idx[t] = res;
              p[t] = w;
              que.push(t);
              t = f(es[mate[t]].u);
            }
          } else if (idx[mate[y]] != res) {
            es[y] = {-1, -1};
            idx[mate[y]] = res;
            p[mate[y]] = y;
            es[mate[y]] = {x, -1};
            que.push(mate[y]);
          }
        }
    }
    return false;
  }

 public:
  MatchingGeneral(int n)
      : g(n), n(n), res(0), mate(n, -1), idx(n, -1), p(n), es(n) {}
  void add_edge(int u, int v) {
    g[u].emplace_back(v);
    g[v].emplace_back(u);
  }
  pair<int, vector<int>> get_matching() {
    for (int i = 0; i < n; i++)
      if (mate[i] == -1) res += check(i);
    return make_pair(res, mate);
  }
};
