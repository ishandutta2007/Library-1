/**
 * @title 最小費用流
 * @category グラフ
 * @brief PrimalDual
 * @brief O((f+F')ElogV)
 * @brief 負辺除去あり
 * @brief 返り値:{流量f流したときの最小コスト,そもそも流量f流せたか(bool)}
 */
// verify用:
// https://codeforces.com/contest/316/problem/C2 (Radix-HeapでないとTLEした)
// https://atcoder.jp/contests/geocon2013/tasks/geocon2013_b (コスト実数)

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

template <typename flow_t, typename cost_t,
          typename Heap
          = priority_queue<pair<cost_t, int>, vector<pair<cost_t, int>>,
                           greater<pair<cost_t, int>>>>
struct MinCostFlow {
  struct Edge {
    int dst;
    flow_t capacity;
    cost_t cost;
    size_t rev;
    bool isrev;
  };

 private:
  static constexpr double EPS = 1e-7;
  int n, S, T;
  cost_t neg;
  vector<vector<Edge>> adj;
  vector<flow_t> d;
  vector<cost_t> potential, dist;
  vector<int> prev;

 private:
  void dijkstra() {
    Heap que;
    dist.assign(n, -2);
    dist[S] = 0;
    que.emplace(0, S);
    while (!que.empty()) {
      auto a = que.top();
      que.pop();
      int v = a.second;
      if (dist[v] < a.first) continue;
      if (v == T) break;
      for (Edge &e : adj[v]) {
        cost_t nextcost = e.cost + potential[v] - potential[e.dst];
        if (e.capacity == 0) continue;
        if (abs(nextcost) < EPS) nextcost = 0;
        if ((dist[e.dst] < -1 || dist[e.dst] > dist[v] + nextcost)) {
          dist[e.dst] = dist[v] + nextcost;
          prev[e.dst] = e.rev;
          que.emplace(dist[v] + nextcost, e.dst);
        }
      }
    }
  }
  pair<cost_t, bool> flow(vector<flow_t> d0) {
    cost_t res = 0;
    potential.assign(n, 0);
    prev.assign(n, -1);
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
      if (dist[T] < -1) return {0, false};  // no solution
      for (int v = 0; v < n; v++) {
        if (dist[v] > -1 && dist[v] < dist[T] + EPS)
          potential[v] += dist[v] - dist[T];
        // if (dist[v] > -1) potential[v] += dist[v];
      }

      flow_t addflow = f;
      for (int v = T; v != S;) {
        Edge &r = adj[v][prev[v]], &e = adj[r.dst][r.rev];
        addflow = min(addflow, e.capacity);
        v = r.dst;
      }
      f -= addflow;
      res += addflow * (potential[T] - potential[S]);
      for (int v = T; v != S;) {
        Edge &r = adj[v][prev[v]], &e = adj[r.dst][r.rev];
        e.capacity -= addflow;
        r.capacity += addflow;
        v = r.dst;
      }
    }
    return {neg + res, true};
  }

 public:
  MinCostFlow(int n_)
      : n(n_ + 2), S(n_), T(n_ + 1), neg(0), adj(n_ + 2), d(n_ + 2) {}
  void add_edge(int src, int dst, flow_t cap, cost_t cost) {
    if (cost >= 0) {
      adj[src].emplace_back((Edge){dst, cap, cost, adj[dst].size(), 0});
      adj[dst].emplace_back((Edge){src, 0, -cost, adj[src].size() - 1, 1});
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
    for (int i = 0; i < adj.size(); i++)
      for (auto &e : adj[i]) {
        if (e.isrev) continue;
        auto &rev_e = adj[e.dst][e.rev];
        cerr << i << "->" << e.dst << " (flow: " << rev_e.capacity << "/"
             << e.capacity + rev_e.capacity << ") cost:" << e.cost << endl;
      }
  }
};