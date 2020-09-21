/**
 * @title 最小費用流
 * @category グラフ
 *  PrimalDual(容量スケーリング)
 *  O(m^2 log U log n)
 * @see https://misawa.github.io/others/flow/library_design.html
 */
// verify用:
// https://codeforces.com/contest/316/problem/C2
// (Radix-HeapでないとTLEした -> 容量スケーリングならTLEしない)
// https://atcoder.jp/contests/geocon2013/tasks/geocon2013_b (コスト実数)

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

template <class flow_t, class cost_t,
          typename Heap = priority_queue<pair<cost_t, int>,
                                         vector<pair<cost_t, int>>, greater<>>,
          int obj = 1>
class MinCostFlow {
  class Edge {
    friend class MinCostFlow;
    int dst;
    flow_t flow, cap;
    cost_t cost;
    int rev;

   public:
    Edge() = default;
    Edge(const int dst, const flow_t cap, const cost_t cost, const int rev)
        : dst(dst), flow(0), cap(cap), cost(cost), rev(rev) {}
    flow_t residual_cap() const { return cap - flow; }
  };

 public:
  class EdgePtr {
    friend class MinCostFlow;
    const MinCostFlow *instance;
    int v;
    int e;
    EdgePtr(const MinCostFlow *const instance, const int v, const int e)
        : instance(instance), v(v), e(e) {}
    const Edge &edge() const { return instance->adj[v][e]; }
    const Edge &rev() const {
      const Edge &e = edge();
      return instance->adj[e.dst][e.rev];
    }

   public:
    EdgePtr() = default;
    int src() const { return v; }
    int dst() const { return edge().dst; }
    flow_t flow() const { return edge().flow; }
    flow_t lower() const { return -rev().cap; }
    flow_t upper() const { return edge().cap; }
    cost_t cost() const { return edge().cost; }
    cost_t gain() const { return -edge().cost; }
  };

 private:
  int n;
  vector<vector<Edge>> adj;
  vector<flow_t> b;

 public:
  MinCostFlow() : n(0) {}
  int add_vertex() {
    n++;
    adj.resize(n);
    b.resize(n);
    return n - 1;
  }
  vector<int> add_vertices(const int size) {
    vector<int> ret(size);
    iota(begin(ret), end(ret), n);
    n += size;
    adj.resize(n);
    b.resize(n);
    return ret;
  }
  EdgePtr add_edge(const int src, const int dst, const flow_t lower,
                   const flow_t upper, const cost_t cost) {
    const int e = adj[src].size(), re = src == dst ? e + 1 : adj[dst].size();
    assert(lower <= upper);
    adj[src].emplace_back(Edge{dst, upper, cost * obj, re});
    adj[dst].emplace_back(Edge{src, -lower, -cost * obj, e});
    return EdgePtr{this, src, e};
  }
  void add_supply(const int v, const flow_t amount) { b[v] += amount; }
  void add_demand(const int v, const flow_t amount) { b[v] -= amount; }

 private:
  const cost_t UNREACHABLE = numeric_limits<cost_t>::max();
  const cost_t EPS = 1e-7;
  cost_t farthest;
  vector<cost_t> potential;
  vector<cost_t> dist;
  vector<int> excess_vs, deficit_vs;
  vector<int> prev;
  Heap pq;
  Edge &rev(const Edge &e) { return adj[e.dst][e.rev]; }
  void push(Edge &e, const flow_t amount) {
    e.flow += amount;
    adj[e.dst][e.rev].flow -= amount;
  }
  cost_t residual_cost_t(const int src, const int dst, const Edge &e) {
    return e.cost + potential[src] - potential[dst];
  }
  bool dual(const flow_t delta) {
    dist.assign(n, UNREACHABLE);
    prev.assign(n, -1);
    excess_vs.erase(remove_if(begin(excess_vs), end(excess_vs),
                              [&](const int v) { return b[v] < delta; }),
                    end(excess_vs));
    deficit_vs.erase(remove_if(begin(deficit_vs), end(deficit_vs),
                               [&](const int v) { return b[v] > -delta; }),
                     end(deficit_vs));
    for (const auto v : excess_vs) pq.emplace(dist[v] = 0, v);
    farthest = 0;
    size_t deficit_count = 0;
    while (!pq.empty()) {
      cost_t d;
      int u;
      tie(d, u) = pq.top();
      pq.pop();
      if (dist[u] - d + EPS < 0) continue;
      farthest = d;
      if (b[u] <= -delta) deficit_count++;
      if (deficit_count >= deficit_vs.size()) break;
      for (auto &e : adj[u]) {
        if (e.residual_cap() < delta) continue;
        const auto v = e.dst;
        const auto new_dist = d + residual_cost_t(u, v, e);
        if (new_dist - dist[v] + EPS >= 0) continue;
        pq.emplace(dist[v] = new_dist, v);
        prev[v] = e.rev;
      }
    }
    pq = decltype(pq)();
    for (int v = 0; v < n; v++) potential[v] += min(dist[v], farthest);
    return deficit_count > 0;
  }
  void primal(const flow_t delta) {
    for (const auto t : deficit_vs) {
      if (dist[t] - farthest - EPS > 0) continue;
      flow_t f = -b[t];
      int v;
      for (v = t; prev[v] != -1;) {
        Edge &r = adj[v][prev[v]], &e = adj[r.dst][r.rev];
        f = min(f, e.residual_cap());
        v = r.dst;
      }
      f = min(f, b[v]);
      f -= f % delta;
      if (f <= 0) continue;
      for (v = t; prev[v] != -1;) {
        Edge &r = adj[v][prev[v]], &e = adj[r.dst][r.rev];
        push(e, f);
        if (e.residual_cap() <= 0) prev[v] = -1;
        v = r.dst;
      }
      b[t] += f;
      b[v] -= f;
    }
  }
  void saturate_negative(const flow_t delta) {
    excess_vs.clear();
    deficit_vs.clear();
    for (auto &es : adj)
      for (auto &e : es) {
        flow_t rcap = e.residual_cap();
        rcap -= rcap % delta;
        int src = adj[e.dst][e.rev].dst, dst = e.dst;
        const cost_t rcost_t = residual_cost_t(src, dst, e);
        if (rcost_t + EPS < 0 || rcap < 0) {
          push(e, rcap);
          b[src] -= rcap;
          b[dst] += rcap;
        }
      }
    for (int v = 0; v < n; v++)
      if (b[v] != 0) (b[v] > 0 ? excess_vs : deficit_vs).emplace_back(v);
  }

 public:
  pair<bool, cost_t> flow_run() {
    potential.resize(n);
    flow_t inf_flow = 1;
    for (const auto t : b) inf_flow = max({inf_flow, t, -t});
    for (const auto &es : adj)
      for (const auto &e : es)
        inf_flow = max({inf_flow, e.residual_cap(), -e.residual_cap()});
    flow_t delta = 1;
    while (delta < inf_flow) delta *= 2;
    for (; delta; delta /= 2) {
      saturate_negative(delta);
      while (dual(delta)) primal(delta);
    }
    cost_t value = 0;
    for (const auto &es : adj)
      for (const auto &e : es) value += e.flow * e.cost;
    value /= 2;
    if (excess_vs.empty() && deficit_vs.empty()) {
      return {true, value / obj};
    } else {
      return {false, value / obj};
    }
  }
  pair<bool, cost_t> st_flow_run(const int s, const int t, flow_t flow) {
    add_supply(s, flow);
    add_demand(t, flow);
    return flow_run();
  }
  tuple<bool, cost_t, flow_t> max_flow_run(const int s, const int t) {
    assert(s != t);
    flow_t inf_flow = abs(b[s]);
    for (const auto &e : adj[s]) inf_flow += max(e.cap, static_cast<flow_t>(0));
    add_edge(t, s, 0, inf_flow, 0);
    bool status;
    cost_t circulation_value;
    tie(status, circulation_value) = flow_run();
    if (!status) {
      adj[s].pop_back();
      adj[t].pop_back();
      return {status, circulation_value, 0};
    }
    inf_flow = abs(b[s]);
    for (const auto &e : adj[s]) inf_flow += e.residual_cap();
    b[s] += inf_flow;
    b[t] -= inf_flow;
    bool mf_status;
    cost_t mf_value;
    tie(mf_status, mf_value) = flow_run();
    b[s] -= inf_flow;
    b[t] += inf_flow;
    adj[s].pop_back();
    adj[t].pop_back();
    return {true, mf_value, b[t]};
  }
  vector<cost_t> get_potential() {
    fill(begin(potential), end(potential), 0);
    for (int i = 0; i < n; i++)
      for (const auto &es : adj)
        for (const auto &e : es)
          if (e.residual_cap() > 0)
            potential[e.dst] = min(potential[e.dst],
                                   potential[adj[e.dst][e.rev].dst] + e.cost);
    return potential;
  }
  template <class T>
  T get_result_value() {
    T value = 0;
    for (const auto &es : adj)
      for (const auto &e : es) {
        value += (T)(e.flow) * (T)(e.cost);
      }
    value /= (T)2;
    return value;
  }
};

template <class flow_t, class cost_t,
          typename Heap = priority_queue<pair<cost_t, int>,
                                         vector<pair<cost_t, int>>, greater<>>>
using MaxGainFlow = MinCostFlow<flow_t, cost_t, Heap, -1>;