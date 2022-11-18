#pragma once
#include <bits/stdc++.h>
/**
 * @title 最大流 PushRelabel(Gap)
 * @category 最適化問題
 *  O(n^2 √m)
 */
// verify用: https://loj.ac/p/127 (Dinicだと落ちる)
// BEGIN CUT HERE
template <class flow_t, unsigned global_freq = 4, bool use_gap = true,
          bool freeze = false>
struct PushRelabel {
  PushRelabel(std::size_t _n = 0) : n(_n), m(0), adj(n) {}

 protected:
  struct Edge {
    int dst, rev;
    flow_t cap;
  };
  int n, gap, m;
  struct Hque {
    std::vector<std::pair<int, int>> even, odd;
    int se, so;
    void init(int _n) { even.resize(_n), odd.resize(_n), se = so = 0; };
    void clear() { se = so = 0; }
    inline bool empty() const { return se + so == 0; }
    void push(int i, int h) { (h & 1 ? odd[so++] : even[se++]) = {i, h}; }
    inline int highest() const {
      int a = se ? even[se - 1].second : -1, b = so ? odd[so - 1].second : -1;
      return a > b ? a : b;
    }
    inline int pop() {
      if (!se || (so && odd[so - 1].second > even[se - 1].second))
        return odd[--so].first;
      return even[--se].first;
    }
  } hque;
  std::vector<std::vector<Edge>> adj;
  std::vector<int> dist, dcnt;
  std::vector<flow_t> excess;
  inline void calc(int t) {
    if constexpr (global_freq != 0) global_relabeling(t);
    for (int tick = m * global_freq; !hque.empty();) {
      int i = hque.pop(), dnxt = n * 2 - 1;
      if constexpr (use_gap)
        if (dist[i] > gap) continue;
      for (auto &e : adj[i])
        if (e.cap) {
          if (dist[e.dst] == dist[i] - 1) {
            if (push(i, e), excess[i] == 0) break;
          } else if (dist[e.dst] + 1 < dnxt)
            dnxt = dist[e.dst] + 1;
        }
      if (excess[i] > 0) {
        if constexpr (use_gap) {
          if (dnxt != dist[i] && dcnt[dist[i]] == 1 && dist[i] < gap)
            gap = dist[i];
          if (dnxt == gap) gap++;
          while (hque.highest() > gap) hque.pop();
          if (dnxt > gap) dnxt = n;
          if (dist[i] != dnxt) dcnt[dist[i]]--, dcnt[dnxt]++;
        }
        dist[i] = dnxt, hq_push(i);
      }
      if constexpr (global_freq != 0)
        if (--tick == 0) tick = m * global_freq, global_relabeling(t);
    }
  }
  inline void hq_push(int i) {
    if constexpr (!use_gap)
      hque.push(i, dist[i]);
    else if (dist[i] < gap)
      hque.push(i, dist[i]);
  }
  inline void push(int i, Edge &e) {
    flow_t delta = std::min(e.cap, excess[i]);
    excess[i] -= delta, e.cap -= delta;
    excess[e.dst] += delta, adj[e.dst][e.rev].cap += delta;
    if (0 < excess[e.dst] && excess[e.dst] <= delta) hq_push(e.dst);
  }
  inline void global_relabeling(int t) {
    dist.assign(n, n), dist[t] = 0;
    static std::queue<int> q;
    q.push(t), hque.clear();
    if constexpr (use_gap) gap = 1, dcnt.assign(n + 1, 0);
    for (int now; !q.empty();) {
      now = q.front(), q.pop();
      if constexpr (use_gap) gap = dist[now] + 1, dcnt[dist[now]]++;
      if (excess[now] > 0) hque.push(now, dist[now]);
      for (const auto &e : adj[now])
        if (adj[e.dst][e.rev].cap && dist[e.dst] == n)
          dist[e.dst] = dist[now] + 1, q.push(e.dst);
    }
  }
  flow_t flow(int s, int t, flow_t flow_lim, flow_t ret = 0) {
    assert(0 <= s && s < n), assert(0 <= t && t < n), assert(s != t);
    hque.init(n), excess.assign(n, 0), excess[s] += flow_lim;
    excess[t] -= flow_lim, dist.assign(n, 0), dist[s] = n;
    if constexpr (use_gap) gap = 1, dcnt.assign(n + 1, 0), dcnt[0] = n - 1;
    for (auto &e : adj[s]) push(s, e);
    calc(t), ret = excess[t] + flow_lim;
    if constexpr (!freeze) {
      excess[s] += excess[t], excess[t] = 0;
      if constexpr (global_freq != 0) global_relabeling(s);
      calc(s), assert(excess == std::vector<flow_t>(n, 0));
    }
    return ret;
  }
};