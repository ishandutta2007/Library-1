/**
 * @title 最小費用流
 * @category グラフ
 * @brief PrimalDual
 * @brief O((f+F')ElogV)
 * @brief 負辺除去あり
 * @brief 返り値:{流量f流したときの最小コスト,そもそも流量f流せたか(bool)}
 */
// verify用: https://codeforces.com/contest/316/problem/C2

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

template <typename flow_t, typename cost_t>
struct MinCostFlow {
  struct Edge {
    int dst;
    flow_t capacity;
    cost_t cost;
    size_t rev;
    bool isrev;
  };
  struct RadixHeap {
    static constexpr int bit = sizeof(cost_t) * 8;
    array<vector<pair<cost_t, int>>, bit> vs;
    size_t sz;
    cost_t last;
    RadixHeap() : sz(0), last(0) {}
    bool empty() const { return sz == 0; }
    inline int getbit(int a) const { return a ? bit - __builtin_clz(a) : 0; }
    inline int getbit(long long a) const {
      return a ? bit - __builtin_clzll(a) : 0;
    }
    void push(cost_t key, int val) {
      sz++;
      vs[getbit(key ^ last)].emplace_back(key, val);
    }
    pair<cost_t, int> pop() {
      if (vs[0].empty()) {
        int idx = 1;
        while (vs[idx].empty()) idx++;
        last = min_element(vs[idx].begin(), vs[idx].end())->first;
        for (auto &p : vs[idx]) vs[getbit(p.first ^ last)].emplace_back(p);
        vs[idx].clear();
      }
      --sz;
      auto res = vs[0].back();
      vs[0].pop_back();
      return res;
    }
  };

 private:
  const cost_t COST_MAX = numeric_limits<cost_t>::max() / 2;
  int n, S, T;
  cost_t neg;
  vector<vector<Edge>> graph;
  vector<flow_t> d;
  vector<cost_t> p, dist;
  vector<int> prevv, preve;

 private:
  void dijkstra() {
    RadixHeap que;
    dist.assign(n, COST_MAX);
    dist[S] = 0;
    que.push(0, S);
    while (!que.empty()) {
      auto a = que.pop();
      int v = a.second;
      if (v == T) break;
      if (dist[v] < a.first) continue;
      for (int i = 0; i < graph[v].size(); i++) {
        Edge &e = graph[v][i];
        if (e.capacity > 0
            && dist[e.dst] > dist[v] + e.cost + p[v] - p[e.dst]) {
          dist[e.dst] = dist[v] + e.cost + p[v] - p[e.dst];
          prevv[e.dst] = v;
          preve[e.dst] = i;
          que.push(dist[e.dst], e.dst);
        }
      }
    }
  }
  pair<cost_t, bool> flow(vector<flow_t> d0) {
    cost_t res = 0;
    p.assign(n, 0);
    preve.assign(n, -1);
    prevv.assign(n, -1);
    flow_t f = 0;
    for (int i = 0; i < d.size(); i++) {
      if (i < d0.size()) d[i] += d0[i];
      if (d[i] > 0)
        add_edge(S, i, d[i], 0), f += d[i];
      else if (d[i] < 0)
        add_edge(i, T, -d[i], 0);
    }
    while (f > 0) {
      dijkstra();
      if (dist[T] == COST_MAX) return {0, false};  // no solution
      for (int v = 0; v < n; v++)
        if (dist[v] < dist[T]) p[v] += dist[v] - dist[T];
      flow_t d = f;
      for (int v = T; v != S; v = prevv[v])
        d = min(d, graph[prevv[v]][preve[v]].capacity);
      f -= d;
      res += d * (p[T] - p[S]);
      for (int v = T; v != S; v = prevv[v]) {
        Edge &e = graph[prevv[v]][preve[v]];
        e.capacity -= d;
        graph[v][e.rev].capacity += d;
      }
    }
    return {neg + res, true};
  }

 public:
  MinCostFlow(int n_)
      : n(n_ + 2), S(n_), T(n_ + 1), neg(0), graph(n_ + 2), d(n_ + 2) {}
  void add_edge(int src, int dst, flow_t cap, cost_t cost) {
    if (cost >= 0) {
      graph[src].emplace_back((Edge){dst, cap, cost, graph[dst].size(), 0});
      graph[dst].emplace_back((Edge){src, 0, -cost, graph[src].size() - 1, 1});
    } else {
      d[src] -= cap;
      d[dst] += cap;
      neg += cap * cost;
      add_edge(dst, src, cap, -cost);
    }
  }
  pair<cost_t, bool> min_cost_flow(int s, int t, flow_t f) {
    vector<flow_t> d0(n);
    d0[s] = f, d0[t] = -f;
    return flow(d0);
  }
  void output() {
    for (int i = 0; i < graph.size(); i++)
      for (auto &e : graph[i]) {
        if (e.isrev) continue;
        auto &rev_e = graph[e.dst][e.rev];
        cerr << i << "->" << e.dst << " (flow: " << rev_e.capacity << "/"
             << e.capacity + rev_e.capacity << ") cost:" << e.cost << endl;
      }
  }
};