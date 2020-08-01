#define PROBLEM \
  "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_6_A"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "Graph/MaxFlow_Dinic.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, M;
  cin >> N >> M;
  MaxFlow_Dinic<long long> graph;
  graph.add_vertices(N);
  for (int i = 0; i < M; i++) {
    int u, v;
    long long c;
    cin >> u >> v >> c;
    graph.add_edge(u, v, c);
  }
  cout << graph.max_flow(0, N - 1) << endl;
  return 0;
}