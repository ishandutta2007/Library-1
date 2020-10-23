#pragma once
#include <bits/stdc++.h>
/**
 * @title 最大流(Dinic)
 * @category グラフ
 *   O(V^2 E)
 *  link: s->tフローで辺(src,dst)の容量を1増やしたときの最大流の変化
 *  cut: s->tフローで辺(src,dst)の容量を1減らしたときの最大流の変化
 */

// 最小流量制限付き最大流 https://snuke.hatenablog.com/entry/2016/07/10/043918

// BEGIN CUT HERE

template <typename flow_t>
struct MaxFlow_Dinic {
 private:
  struct Edge {
    int dst;
    flow_t cap;
    int rev;
    bool isrev;
  };

 private:
  static constexpr flow_t FLOW_MAX = numeric_limits<flow_t>::max() / 2;
  int n;
  std::vector<std::vector<Edge>> adj;
  std::vector<int> level, iter;

 private:
  int levelize(const int &s, const int &t) {
    level.assign(n, -1);
    level[s] = 0;
    queue<int> que;
    que.push(s);
    while (!que.empty()) {
      int u = que.front();
      que.pop();
      if (u == t) break;
      for (auto &e : adj[u]) {
        if (e.cap > 0 && level[e.dst] < 0) {
          que.push(e.dst);
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
      if (e.cap > 0 && level[u] < level[e.dst]) {
        flow_t f = dfs(e.dst, t, min(cur, e.cap));
        if (f > 0) {
          e.cap -= f;
          r.cap += f;
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
  MaxFlow_Dinic() : n(0) {}
  int add_vertex() {
    adj.resize(++n);
    return n - 1;
  }
  std::vector<int> add_vertices(const int size) {
    std::vector<int> ret(size);
    iota(begin(ret), end(ret), n);
    n += size;
    adj.resize(n);
    return ret;
  }
  void add_edge(int src, int dst, flow_t cap) {
    adj[src].emplace_back((Edge){dst, cap, (int)adj[dst].size(), 0});
    adj[dst].emplace_back((Edge){src, 0, (int)adj[src].size() - 1, 1});
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
        e.cap += 1;
        break;
      }
    return flow(s, t);
  }
  flow_t cut(int src, int dst, int s, int t) {
    for (auto &e : adj[src])
      if (e.dst == dst && !e.isrev) {
        flow_t diff = 0;
        if (e.cap == 0) {
          if (flow(src, dst) == 0) {
            flow(t, dst);
            flow(src, s);
            diff = -1;
          }
          adj[e.dst][e.rev].cap -= 1;
        } else {
          e.cap -= 1;
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
        cerr << i << "->" << e.dst << " (flow: " << rev_e.cap << "/"
             << e.cap + rev_e.cap << ")" << endl;
      }
    }
  }
};