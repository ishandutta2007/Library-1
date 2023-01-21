#define PROBLEM "https://judge.yosupo.jp/problem/min_cost_b_flow"
#include <iostream>
#include <vector>
#include "src/Optimization/MinCostFlow.hpp"
using namespace std;
ostream &operator<<(ostream &stream, const __int128_t &v) {
 if (v == 0) stream << "0";
 __int128_t tmp;
 if (v < 0) stream << "-", tmp= -v;
 else tmp= v;
 std::string s;
 while (tmp) s+= '0' + (tmp % 10), tmp/= 10;
 return std::reverse(s.begin(), s.end()), stream << s;
}
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using MCF= MinCostFlow<NetworkSimplex, long long, long long>;
 int N, M;
 cin >> N >> M;
 MCF graph(N);
 vector<MCF::EdgePtr> edges;
 for (int i= 0; i < N; i++) {
  long long b;
  cin >> b;
  graph.add_supply(i, b);
 }
 for (int i= 0; i < M; i++) {
  long long s, t, l, u, c;
  cin >> s >> t >> l >> u >> c;
  edges.emplace_back(graph.add_edge(s, t, l, u, c));
 }
 if (graph.b_flow()) {
  cout << graph.get_result_value<__int128_t>() << '\n';
  for (int i= 0; i < N; i++) cout << graph.get_potential(i) << '\n';
  for (auto &e: edges) cout << e.flow() << '\n';
 } else {
  cout << "infeasible" << '\n';
 }
 return 0;
}
