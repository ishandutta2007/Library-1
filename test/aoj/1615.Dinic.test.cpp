// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/ICPC/Prelim/1615
// competitive-verifier: TLE 1.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Optimization/MaxFlow.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 for (int n, m; cin >> n >> m, n;) {
  int u[m], v[m];
  for (int i= 0; i < m; i++) cin >> u[i] >> v[i], u[i]--, v[i]--;
  for (int d= 0; d <= n; d++)
   for (int l= n - d; l >= 0; l--) {
    int r= l + d;
    MaxFlowLowerBound<Dinic<long long>> graph;
    int s= graph.add_vertex(), t= graph.add_vertex();
    auto e= graph.add_vertices(m);
    auto w= graph.add_vertices(n);
    for (int i= 0; i < m; i++) {
     graph.add_edge(s, e[i], 0, 1);
     graph.add_edge(e[i], w[u[i]], 0, 1);
     graph.add_edge(e[i], w[v[i]], 0, 1);
    }
    for (int i= 0; i < n; i++) graph.add_edge(w[i], t, l, r);
    if (graph.maxflow(s, t) == m) {
     cout << l << " " << r << '\n';
     d= n + 1;
     break;
    }
   }
 }
 return 0;
}