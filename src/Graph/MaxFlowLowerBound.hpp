#pragma once
#include <bits/stdc++.h>
/**
 * @title 最小流量制限付き最大流インターフェース
 * @category グラフ
 * アルゴリズム(Dinic等)はclassテンプレートで受け取る
 * @see https://snuke.hatenablog.com/entry/2016/07/10/043918
 * 容量は負も可 (逆向きに流れる意味)
 */

// BEGIN CUT HERE

template <typename FlowAlgo>
class MaxFlowLowerBound : public FlowAlgo {
  using Edge = typename FlowAlgo::Edge;
  using flow_t = decltype(Edge::cap);
  std::vector<flow_t> in;
  int add_edge(int src, int dst, flow_t cap) {
    int e = this->adj[src].size();
    int re = src == dst ? e + 1 : this->adj[dst].size();
    this->adj[src].push_back(Edge{dst, re, cap});
    this->adj[dst].push_back(Edge{src, e, 0});
    return this->m++, re;
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
  void add_edge(int src, int dst, flow_t lower, flow_t upper) {
    assert(lower <= upper);
    if (upper < 0) return add_edge(dst, src, -upper, -lower);
    src += 2, dst += 2;
    assert(0 <= src && src < this->n);
    assert(0 <= dst && dst < this->n);
    this->m++;
    int e = this->adj[src].size();
    int re = src == dst ? e + 1 : this->adj[dst].size();
    if (lower <= 0) {
      this->adj[src].push_back(Edge{dst, re, upper});
      this->adj[dst].push_back(Edge{src, e, -lower});
    } else {
      in[src - 2] -= lower, in[dst - 2] += lower;
      this->adj[src].push_back(Edge{dst, re, upper - lower});
      this->adj[dst].push_back(Edge{src, e, 0});
    }
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