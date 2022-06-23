#pragma once
#include <bits/stdc++.h>
/**
 * @title 最小費用流
 * @category 最適化問題
 *  ネットワーク単体法
 */
// verify用:
// https://atcoder.jp/contests/geocon2013/tasks/geocon2013_b (コスト実数)

// BEGIN CUT HERE

template <typename flow_t, typename cost_t, std::int_least8_t obj = 1>
class NetworkSimplex {
  struct Node {
    int par, pred;
    flow_t sup;
    cost_t pi;
  };
  struct Edge {
    int u, v;
    flow_t low, up, flow;
    cost_t cost;
    std::int_least8_t state = 1;
  };
  int n, m = 0;
  std::vector<Node> ns;
  std::vector<Edge> es;
  std::vector<int> bfs, next, prev;
  inline void link(int u, int v) { next[u] = v, prev[v] = u; }
  inline void link(int u, int v, int w) { link(u, v), link(v, w); }
  inline auto opp_cost(int e) const {
    return es[e].cost + ns[es[e].u].pi - ns[es[e].v].pi;
  }
  inline void pivot(int in_arc) {
    int u_in = es[in_arc].u, v_in = es[in_arc].v, u, e, a = u_in, b = v_in;
    while (a != b)
      a = ns[a].par == -1 ? v_in : ns[a].par,
      b = ns[b].par == -1 ? u_in : ns[b].par;
    if (es[in_arc].state == -1) std::swap(u_in, v_in);
    int lca = a, side = 0, u_out = -1, i = 0, S = 0;
    flow_t delta = es[in_arc].up;
    for (u = u_in; u != lca && delta > 0; u = ns[u].par) {
      flow_t d = u == es[e = ns[u].pred].v ? es[e].up - es[e].flow : es[e].flow;
      if (delta > d) delta = d, u_out = u, side = 1;
    }
    for (u = v_in; u != lca; u = ns[u].par) {
      flow_t d = u == es[e = ns[u].pred].u ? es[e].up - es[e].flow : es[e].flow;
      if (delta >= d) delta = d, u_out = u, side = -1;
    }
    if (delta > 0) {
      es[in_arc].flow += delta *= es[in_arc].state;
      for (u = es[in_arc].u; u != lca; u = ns[u].par)
        es[e].flow += u == es[e = ns[u].pred].u ? -delta : delta;
      for (u = es[in_arc].v; u != lca; u = ns[u].par)
        es[e].flow += u == es[e = ns[u].pred].u ? delta : -delta;
    }
    if (side == 0) return es[in_arc].state *= -1, void();
    int out_arc = ns[u_out].pred, p;
    es[in_arc].state = 0, es[out_arc].state = es[out_arc].flow ? -1 : 1;
    if (side == -1) std::swap(u_in, v_in);
    for (u = u_in; u != u_out; u = ns[u].par) bfs[S++] = u;
    assert(S <= n);
    for (i = S; i--;)
      ns[p = ns[u].par].par = u = bfs[i], ns[p].pred = ns[u].pred,
             link(prev[p], next[p]), link(prev[u + n + 1], p, u + n + 1);
    link(prev[u_in], next[u_in]), link(prev[v_in + n + 1], u_in, v_in + n + 1);
    ns[u_in].par = v_in, ns[u_in].pred = in_arc;
    cost_t pi_delta =
        u_in == es[in_arc].u ? -opp_cost(in_arc) : opp_cost(in_arc);
    for (i = 0, S = 1, bfs[0] = u_in; i < S; i++) {
      ns[u = bfs[i]].pi += pi_delta;
      for (int v = next[u + n + 1]; v != u + n + 1; v = next[v]) bfs[S++] = v;
    }
  }
  void calc() {
    cost_t inf_cost = 1;
    for (int e = 0; e < m; e++)
      es[e].flow = 0, es[e].state = 1, es[e].up -= es[e].low,
      ns[es[e].u].sup -= es[e].low, ns[es[e].v].sup += es[e].low,
      inf_cost += std::abs(es[e].cost);
    ns[n] = {-1, -1, 0, 0}, es.resize(m + n), bfs.resize(n + 1);
    next.resize(2 * n + 2), std::iota(next.begin() + n + 1, next.end(), n + 1);
    prev.resize(2 * n + 2), std::iota(prev.begin() + n + 1, prev.end(), n + 1);
    for (int u = 0, e = m; u < n; u++, e++) {
      ns[u].par = n, ns[u].pred = e, link(prev[n + n + 1], u, n + n + 1);
      if (auto supply = ns[u].sup; supply >= 0) {
        ns[u].pi = -inf_cost;
        es[e] = {u, n, 0, supply, supply, inf_cost, 0};
      } else {
        ns[u].pi = inf_cost;
        es[e] = {n, u, 0, -supply, -supply, inf_cost, 0};
      }
    }
    int block_size =
        std::max(int(std::ceil(std::sqrt(m + n))), std::min(10, n + 1));
    for (int e = 0, in_arc, cnt, seen;; pivot(in_arc)) {
      cost_t minimum = 0;
      for (in_arc = -1, cnt = block_size, seen = m + n; seen--;
           e = e + 1 == m + n ? 0 : e + 1) {
        if (minimum > es[e].state * opp_cost(e))
          minimum = es[e].state * opp_cost(e), in_arc = e;
        if (--cnt == 0 && minimum < 0) break;
        if (cnt == 0) cnt = block_size;
      }
      if (in_arc == -1) break;
    }
    for (int e = 0; e < m; e++)
      es[e].flow += es[e].low, es[e].up += es[e].low,
          ns[es[e].u].sup += es[e].low, ns[es[e].v].sup -= es[e].low;
  }

 public:
  NetworkSimplex(int n = 0) : n(n), ns(n + 1) {}
  int add_vertex() { return ns.resize(n + 2), n++; }
  std::vector<int> add_vertices(const int size) {
    std::vector<int> ret(size);
    std::iota(ret.begin(), ret.end(), n);
    return ns.resize((n += size) + 1), ret;
  }
  class EdgePtr {
    friend class NetworkSimplex;
    const NetworkSimplex *instance;
    int e;
    EdgePtr(const NetworkSimplex *const instance, const int e)
        : instance(instance), e(e) {}
    const Edge &edge() const { return instance->es[e]; }

   public:
    EdgePtr() = default;
    int src() const { return edge().u; }
    int dst() const { return edge().v; }
    flow_t flow() const { return edge().flow; }
    flow_t lower() const { return edge().low; }
    flow_t upper() const { return edge().up; }
    cost_t cost() const { return edge().cost; }
    cost_t gain() const { return -edge().cost; }
  };
  EdgePtr add_edge(int u, int v, flow_t low, flow_t up, cost_t cost) {
    assert(0 <= u && u < n && 0 <= v && v < n);
    es.push_back({u, v, low, up, 0, obj * cost});
    return EdgePtr{this, m++};
  }
  void add_supply(int u, flow_t supply) { ns[u].sup = supply; }
  void add_demand(int u, flow_t demand) { ns[u].sup = -demand; }
  auto get_supply(int u) const { return ns[u].sup; }
  auto get_potential(int u) const { return ns[u].pi; }
  template <typename cost_sum_t = cost_t>
  auto get_result_value() const {
    cost_sum_t sum = 0;
    for (int e = 0; e < m; e++) {
      sum += es[e].flow * cost_sum_t(es[e].cost);
    }
    return sum * obj;
  }
  bool b_flow() {
    flow_t sum_supply = 0;
    for (int u = 0; u < n; u++) sum_supply += ns[u].sup;
    if (sum_supply != 0) return false;
    calc();
    for (int e = m; e < m + n; e++)
      if (es[e].flow != 0) return es.resize(m), false;
    return es.resize(m), true;
  }
};

template <template <class, class, std::int_least8_t> class FlowAlgo,
          typename flow_t, typename cost_t>
using MinCostFlow = FlowAlgo<flow_t, cost_t, 1>;
template <template <class, class, std::int_least8_t> class FlowAlgo,
          typename flow_t, typename cost_t>
using MaxGainFlow = FlowAlgo<flow_t, cost_t, -1>;
