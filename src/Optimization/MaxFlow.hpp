#pragma once
#include <bits/stdc++.h>
/**
 * @title 最大流インターフェース
 * @category 最適化問題
 * アルゴリズム(Dinic等)はclassテンプレートで受け取る
 * EdgePtr:
 *  change_cap: 容量を変更, それに伴うフローのはみ出しを出力
 * 双方向辺も可
 */

// BEGIN CUT HERE
template <typename FlowAlgo>
struct MaxFlow : public FlowAlgo {
  using FlowAlgo::FlowAlgo;
  using Edge = typename FlowAlgo::Edge;
  using flow_t = decltype(Edge::cap);
  int add_vertex() { return this->adj.resize(++this->n), this->n - 1; }
  std::vector<int> add_vertices(const std::size_t size) {
    std::vector<int> ret(size);
    std::iota(ret.begin(), ret.end(), this->n);
    return this->adj.resize(this->n += size), ret;
  }
  struct EdgePtr {
    friend class MaxFlow;
    MaxFlow *instance;
    int v, e;
    bool bidir;
    Edge &edge() { return instance->adj[v][e]; }
    Edge &rev() {
      Edge &e = edge();
      return instance->adj[e.dst][e.rev];
    }
    EdgePtr(MaxFlow *instance, int v, int e, bool d)
        : instance(instance), v(v), e(e), bidir(d) {}

   public:
    EdgePtr() = default;
    int src() { return v; }
    int dst() { return edge().dst; }
    bool is_direct() const { return !bidir; }
    flow_t flow() { return cap() - edge().cap; }
    flow_t cap() { return (edge().cap + rev().cap) / (1 + bidir); }
    flow_t change_cap(flow_t new_cap, int s, int t) {
      assert(0 <= new_cap);
      Edge &e = edge(), &re = rev();
      flow_t diff = new_cap - cap(), extrusion = std::abs(flow()) - new_cap;
      if (extrusion <= 0) return e.cap += diff, re.cap += diff * bidir, 0;
      int sr = src(), ds = dst();
      if (flow() < 0) std::swap(sr, ds);
      if (bidir) {
        if (sr == src())
          re.cap += 2 * diff + e.cap, e.cap = 0;
        else
          e.cap += 2 * diff + re.cap, re.cap = 0;
      } else
        re.cap += diff;
      extrusion -= instance->maxflow(sr, ds, extrusion);
      if (ds != t) instance->maxflow(t, ds, extrusion);
      if (sr != s) instance->maxflow(sr, s, extrusion);
      return extrusion;
    }
  };
  EdgePtr add_edge(int src, int dst, flow_t cap, bool bidir = false) {
    assert(0 <= src && src < this->n), assert(0 <= dst && dst < this->n);
    assert(0 <= cap);
    int e = this->adj[src].size();
    int re = src == dst ? e + 1 : this->adj[dst].size();
    return this->adj[src].push_back(Edge{dst, re, cap}),
           this->adj[dst].push_back(Edge{src, e, cap * bidir}), this->m++,
           EdgePtr{this, src, e, bidir};
  }
  flow_t maxflow(int s, int t) {
    return maxflow(s, t, std::numeric_limits<flow_t>::max());
  }
  flow_t maxflow(int s, int t, flow_t flow_lim) {
    return this->flow(s, t, flow_lim);
  }
  std::vector<bool> mincut(int s) {
    std::vector<bool> visited(this->n);
    static std::queue<int> que;
    for (que.push(s); !que.empty();) {
      s = que.front(), que.pop(), visited[s] = true;
      for (const auto &e : this->adj[s])
        if (e.cap && !visited[e.dst]) visited[e.dst] = true, que.push(e.dst);
    }
    return visited;
  }
};