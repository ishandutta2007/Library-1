/**
 * @title 最大流(Dinic)
 * @category グラフ
 * @brief  O(V^2 E)
 * @brief link: s->tフローで辺(src,dst)の容量を1増やしたときの最大流の変化
 * @brief cut: s->tフローで辺(src,dst)の容量を1減らしたときの最大流の変化
 */

// 最小流量制限付き最大流 https://snuke.hatenablog.com/entry/2016/07/10/043918
#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

template <typename flow_t>
struct MaxFlow_Dinic {
  struct Edge {
    int dst;
    flow_t capacity;
    size_t rev;
    bool isrev;
  };

 private:
  static constexpr flow_t FLOW_MAX = numeric_limits<flow_t>::max() / 2;
  int n;
  vector<vector<Edge>> adj;
  vector<int> level, iter;

 private:
  int levelize(const int &s, const int &t) {
    level.assign(n, -1);
    level[s] = 0;
    queue<int> Q;
    Q.push(s);
    while (!Q.empty()) {
      int u = Q.front();
      Q.pop();
      if (u == t) break;
      for (auto &e : adj[u]) {
        if (e.capacity > 0 && level[e.dst] < 0) {
          Q.push(e.dst);
          level[e.dst] = level[u] + 1;
        }
      }
    }
    return level[t];
  }
  flow_t dfs(int u, const int &t, flow_t cur) {
    if (u == t) return cur;
    for (int &i = iter[u]; i < adj[u].size(); ++i) {
      Edge &e = adj[u][i], &r = adj[e.dst][e.rev];
      if (e.capacity > 0 && level[u] < level[e.dst]) {
        flow_t f = dfs(e.dst, t, min(cur, e.capacity));
        if (f > 0) {
          e.capacity -= f;
          r.capacity += f;
          return f;
        }
      }
    }
    return flow_t(0);
  }
  flow_t flow(int s, int t) {
    if (levelize(s, t) < 0) return 0;
    iter.assign(adj.size(), 0);
    return dfs(s, t, 1);
  }

 public:
  MaxFlow_Dinic(int n) : n(n), adj(n) {}
  void add_edge(int src, int dst, flow_t cap) {
    adj[src].emplace_back((Edge){dst, cap, adj[dst].size(), 0});
    adj[dst].emplace_back((Edge){src, 0, adj[src].size() - 1, 1});
  }
  flow_t max_flow(int s, int t, flow_t lim = FLOW_MAX) {
    flow_t flow = 0;
    while (levelize(s, t) >= 0) {
      iter.assign(n, 0);
      for (flow_t f; (f = dfs(s, t, lim)) > 0;) flow += f, lim -= f;
    }
    return flow;
  }
  flow_t link(int src, int dst, int s, int t) {
    for (auto &e : adj[src])
      if (e.dst == dst && !e.isrev) {
        e.capacity += 1;
        break;
      }
    return flow(s, t);
  }
  flow_t cut(int src, int dst, int s, int t) {
    for (auto &e : adj[src])
      if (e.dst == dst && !e.isrev) {
        flow_t diff = 0;
        if (e.capacity == 0) {
          if (flow(src, dst) == 0) {
            flow(t, dst);
            flow(src, s);
            diff = -1;
          }
          adj[e.dst][e.rev].capacity -= 1;
        } else {
          e.capacity -= 1;
        }
        return diff;
      }
    assert(false);  // no edge
  }
  void output() {
    for (int i = 0; i < adj.size(); i++) {
      for (auto &e : adj[i]) {
        if (e.isrev) continue;
        auto &rev_e = adj[e.dst][e.rev];
        cerr << i << "->" << e.dst << " (flow: " << rev_e.capacity << "/"
             << e.capacity + rev_e.capacity << ")" << endl;
      }
    }
  }
};