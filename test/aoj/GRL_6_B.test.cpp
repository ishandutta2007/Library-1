#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/6/GRL_6_B"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Graph/MinCostFlow.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int V, E, F;
  cin >> V >> E >> F;
  MinCostFlow<int, int> graph;
  graph.add_vertices(V);
  while (E--) {
    int u, v, c, d;
    cin >> u >> v >> c >> d;
    graph.add_edge(u, v, 0, c, d);
  }
  auto ans = graph.st_flow_run(0, V - 1, F);
  cout << (ans.first ? ans.second : -1) << endl;
  return 0;
}