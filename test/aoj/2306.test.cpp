// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/2306
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Graph/CliqueProblem.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int n, m;
 cin >> n >> m;
 CliqueProblem graph(n);
 vector adj(n, vector(n, 0));
 for (int i= 0; i < m; ++i) {
  int u, v, f;
  cin >> u >> v;
  --u, --v;
  cin >> f;
  adj[v][u]= adj[u][v]= f, graph.add_edge(u, v);
 }
 constexpr int INF= 1'000'010;
 long long ans= 0;
 graph.enumerate_cliques([&](const auto &clique) {
  if (clique.size() == 1) return;
  long long a= 0;
  for (int u: clique) {
   int b= INF;
   for (int v: clique)
    if (u != v) b= min(b, adj[u][v]);
   a+= b;
  }
  ans= max(ans, a);
 });
 cout << ans << '\n';
 return 0;
}