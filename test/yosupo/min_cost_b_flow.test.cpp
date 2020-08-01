#define PROBLEM "https://judge.yosupo.jp/problem/min_cost_b_flow"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "Graph/MinCostFlow.hpp"
#undef call_from_test

ostream &operator<<(ostream &stream, const __int128_t &v) {
  if (v == 0) stream << "0";
  __int128_t tmp;
  if (v < 0) {
    stream << "-";
    tmp = -v;
  } else {
    tmp = v;
  }
  std::string s;
  while (tmp) {
    s += '0' + (tmp % 10);
    tmp /= 10;
  }
  std::reverse(s.begin(), s.end());
  stream << s;
  return stream;
}

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using MCF = MinCostFlow<long long, long long>;
  MCF graph;
  vector<MCF::EdgePtr> edges;
  int N, M;
  cin >> N >> M;
  graph.add_vertices(N);
  for (int i = 0; i < N; i++) {
    long long b;
    cin >> b;
    graph.add_supply(i, b);
  }
  for (int i = 0; i < M; i++) {
    long long s, t, l, u, c;
    cin >> s >> t >> l >> u >> c;
    edges.emplace_back(graph.add_edge(s, t, l, u, c));
  }
  bool isok = graph.flow_run().first;
  if (isok) {
    cout << graph.get_result_value<__int128_t>() << endl;
    auto potential = graph.get_potential();
    for (auto p : potential) cout << p << endl;
    for (auto &e : edges) cout << e.flow() << endl;
  } else {
    cout << "infeasible" << endl;
  }
  return 0;
}
