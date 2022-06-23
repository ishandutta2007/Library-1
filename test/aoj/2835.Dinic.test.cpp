#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/challenges/sources/VPC/RUPC/2835"
#include <bits/stdc++.h>
#include "src/Optimization/Dinic.hpp"
#include "src/Optimization/MaxFlow.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  int V, E;
  cin >> V >> E;
  using MF = MaxFlow<Dinic<long long>>;
  MF graph(V);
  vector<MF::EdgePtr> edges;
  for (int i = 0; i < E; i++) {
    int s, t, c;
    cin >> s >> t >> c;
    edges.emplace_back(graph.add_edge(s, t, c, true));
  }
  long long ans = graph.maxflow(0, V - 1);
  auto S = graph.mincut(0);
  for (auto &e : edges)
    if (e.cap() == 1) {
      if (e.change_cap(0, 0, V - 1)) {
        ans--;
        break;
      }
      e.change_cap(1, 0, V - 1);
    }
  cout << (ans > 10000 ? -1 : ans) << '\n';
  return 0;
}
