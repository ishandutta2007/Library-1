#define PROBLEM "https://judge.yosupo.jp/problem/assignment"
#include <bits/stdc++.h>
#include "src/Optimization/MinCostFlow.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using MCF = MinCostFlow<NetworkSimplex, long long, long long>;
  int N;
  cin >> N;
  MCF graph;
  vector<vector<MCF::EdgePtr>> edges(N, vector<MCF::EdgePtr>(N));
  auto v_left = graph.add_vertices(N);
  auto v_right = graph.add_vertices(N);
  for (int i = 0; i < N; i++) {
    graph.add_supply(v_left[i], 1);
    graph.add_demand(v_right[i], 1);
  }
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      long long A;
      cin >> A;
      edges[i][j] = graph.add_edge(v_left[i], v_right[j], 0, 1, A);
    }
  }
  graph.b_flow();
  cout << graph.get_result_value() << endl;
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      if (edges[i][j].flow()) cout << j << " \n"[i == N - 1];
  return 0;
}
