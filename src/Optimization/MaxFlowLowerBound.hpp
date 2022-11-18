#pragma once
#include <bits/stdc++.h>
/**
 * @title 最小流量制限付き最大流インターフェース
 * @category 最適化問題
 * アルゴリズム(Dinic等)はclassテンプレートで受け取る
 * @see https://snuke.hatenablog.com/entry/2016/07/10/043918
 * 容量は負も可 (逆向きに流れる意味)
 */

// verify用: https://yukicoder.me/problems/no/459

// BEGIN CUT HERE
template <typename FlowAlgo>
class MaxFlowLowerBound : public FlowAlgo {
  using Edge = typename FlowAlgo::Edge;
  using flow_t = decltype(Edge::cap);
  std::vector<flow_t> in;
  int add_edge(int src, int dst, flow_t cap) {
    int e = this->adj[src].size();
    int re = src == dst ? e + 1 : this->adj[dst].size();
    return this->adj[src].push_back(Edge{dst, re, cap}),
           this->adj[dst].push_back(Edge{src, e, 0}), this->m++, re;
  }

 public:
  MaxFlowLowerBound(std::size_t n = 0) : FlowAlgo(n + 2), in(n) {}
  int add_vertex() {
    return this->adj.resize(++this->n), in.resize(this->n - 2, 0), this->n - 3;
  }
  std::vector<int> add_vertices(const std::size_t size) {
    std::vector<int> ret(size);
    std::iota(ret.begin(), ret.end(), this->n - 2);
    return this->adj.resize(this->n += size), in.resize(this->n - 2, 0), ret;
  }
  struct EdgePtr {
    friend class MaxFlowLowerBound;
    MaxFlowLowerBound *instance;
    int v, e;
    flow_t u;
    const Edge &edge() { return instance->adj[v][e]; }
    const Edge &rev() {
      Edge &e = edge();
      return instance->adj[e.dst][e.rev];
    }
    EdgePtr(MaxFlowLowerBound *instance, int v, int e, flow_t u)
        : instance(instance), v(v), e(e), u(u) {}

   public:
    EdgePtr() = default;
    int src() const { return v; }
    int dst() const { return edge().dst; }
    flow_t flow() const { return u - edge().cap; }
    flow_t lower() const { return flow() - rev().cap; }
    flow_t upper() const { return u; }
  };
  EdgePtr add_edge(int src, int dst, flow_t lower, flow_t upper) {
    assert(lower <= upper), src += 2, dst += 2;
    assert(0 <= src && src < this->n), assert(0 <= dst && dst < this->n);
    this->m++;
    int e = this->adj[src].size(),
        re = src == dst ? e + 1 : this->adj[dst].size();
    if (lower * upper <= 0) {
      this->adj[src].push_back(Edge{dst, re, upper});
      this->adj[dst].push_back(Edge{src, e, -lower});
    } else if (lower > 0) {
      in[src - 2] -= lower, in[dst - 2] += lower;
      this->adj[src].push_back(Edge{dst, re, upper - lower});
      this->adj[dst].push_back(Edge{src, e, 0});
    } else {
      in[src - 2] -= upper, in[dst - 2] += upper;
      this->adj[src].push_back(Edge{dst, re, 0});
      this->adj[dst].push_back(Edge{src, e, upper - lower});
    }
    return EdgePtr(this, src, e, upper);
  }
  flow_t maxflow(int s, int t) {
    static constexpr flow_t INF = std::numeric_limits<flow_t>::max();
    flow_t sum = 0;
    for (int i = this->n - 2; i--;) {
      if (in[i] > 0) add_edge(0, i + 2, in[i]), sum += in[i];
      if (in[i] < 0) add_edge(i + 2, 1, -in[i]);
    }
    int re = add_edge(t += 2, s += 2, INF);
    if (this->flow(0, 1, INF) < sum) return -1;  // no solution
    return this->flow(s, t, INF) + this->adj[s][re].cap;
  }
};