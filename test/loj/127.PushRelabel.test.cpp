// competitive-verifier: PROBLEM https://loj.ac/p/127
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

// Dinic だと TLE する

#include <iostream>
#include "src/Optimization/MaxFlow.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int n, m, s, t;
 cin >> n >> m >> s >> t, --s, --t;
 MaxFlow<PushRelabel<long long>> graph(n);
 for (int i= 0; i < m; i++) {
  int u, v, c;
  cin >> u >> v >> c;
  graph.add_edge(--u, --v, c);
 }
 cout << graph.maxflow(s, t) << '\n';
 return 0;
}