#pragma once
#include <bits/stdc++.h>
/**
 * @title Dinic
 * @category アルゴリズム
 *  O(n^2 m)
 * @see https://misawa.github.io/others/flow/dinic_time_complexity.html
 */

// BEGIN CUT HERE

template <class flow_t>
struct Dinic {
  Dinic(std::size_t _n = 0) : n(_n), m(0), adj(n) {}

 protected:
  struct Edge {
    int dst, rev;
    flow_t cap;
  };
  int n, m;
  std::vector<std::vector<Edge>> adj;
  std::vector<int> level, iter;
  inline void levelize(const int &s, const int &t) {
    level.assign(n, -1), level[s] = 0;
    std::queue<int> que;
    for (que.push(s); !que.empty();) {
      int u = que.front();
      que.pop();
      for (auto &e : adj[u])
        if (e.cap > 0 && level[e.dst] < 0) {
          level[e.dst] = level[u] + 1;
          if (e.dst == t) return;
          que.push(e.dst);
        }
    }
  }
  inline flow_t dfs(int u, const int &s, flow_t cur) {
    if (u == s) return cur;
    flow_t ret = 0;
    for (int &i = iter[u]; i < int(adj[u].size()); i++) {
      Edge &e = adj[u][i], &re = adj[e.dst][e.rev];
      if (level[u] <= level[e.dst] || re.cap == 0) continue;
      flow_t f = dfs(e.dst, s, std::min(cur - ret, re.cap));
      if (f <= 0) continue;
      e.cap += f, re.cap -= f, ret += f;
      if (ret == cur) return ret;
    }
    return level[u] = n, ret;
  }
  flow_t flow(int s, int t, flow_t flow_lim) {
    assert(0 <= s && s < n);
    assert(0 <= t && t < n);
    assert(s != t);
    flow_t ret = 0;
    for (flow_t f; ret < flow_lim; ret += f) {
      if (levelize(s, t), level[t] == -1) break;
      iter.assign(n, 0);
      if (!(f = dfs(t, s, flow_lim - ret))) break;
    }
    return ret;
  }
};
