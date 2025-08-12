// competitive-verifier: PROBLEM https://loj.ac/p/6699
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/Graph/Graph.hpp"
#include "src/Graph/HeavyLightDecomposition.hpp"
#include "src/Graph/Rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int n, q;
 cin >> n >> q;
 vector<int> a(n);
 for (int i= 0; i < n; ++i) cin >> a[i];
 Graph g(n, n - 1);
 vector<long long> z1(n - 1), z2(n - 1);
 for (int i= 0; i < n - 1; ++i) cin >> g[i] >> z1[i] >> z2[i], --g[i];
 HeavyLightDecomposition hld(g, 0);
 auto adje= g.adjacency_edge(0);
 auto put_edge= [&](int, int e, long long x) { return max(0ll, x - z1[e] - z2[e]); };
 auto op= [](long long l, long long r) { return l + r; };
 auto put_vertex= [&](int v, long long x) { return x + a[v]; };
 Rerooting<long long> dp(g, adje, hld, put_edge, op, 0ll, put_vertex);
 vector<long long> d1(n), d2(n), f(n);
 for (int i= 0; i < n; ++i) {
  int v= hld.to_vertex(i), p= hld.parent(v);
  for (int e: adje[v]) {
   int u= g[e].to(v);
   if (u == p) continue;
   d1[u]= d1[v], d2[u]= d2[v];
   if (g[e].first == u) d1[u]+= z1[e], d2[u]+= z2[e];
   else d2[u]+= z1[e], d1[u]+= z2[e];
   f[u]= f[v] + min(dp(0, u), z1[e] + z2[e]);
  }
 }
 while (q--) {
  int u, v;
  cin >> u >> v, --u, --v;
  int l= hld.lca(u, v);
  cout << dp[l] + f[u] + f[v] - f[l] * 2 - (d1[u] - d1[l] + d2[v] - d2[l]) << '\n';
 }
 return 0;
}