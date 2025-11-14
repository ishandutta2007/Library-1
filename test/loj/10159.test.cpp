// competitive-verifier: PROBLEM https://loj.ac/p/10159
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

#include <iostream>
#include "src/Graph/Graph.hpp"
#include "src/Graph/Rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int n;
 cin >> n;
 Graph g(n, n - 1);
 for (auto &e: g) cin >> e;
 auto put_edge= [&](int, int, int d) { return d + 1; };
 auto op= [&](int l, int r) { return max(l, r); };
 auto put_vertex= [&](int, int d) { return d; };
 Rerooting<int> dp(g, put_edge, op, 0, put_vertex);
 int di= 0;
 for (int v= n; v--;) di= max(di, dp[v]);
 auto adj= g.adjacency_vertex(0);
 vector<int> ans;
 for (int v= 0; v < n; ++v) {
  if (adj[v].size() == 1) {
   if (dp[v] == di) ans.push_back(v);
   continue;
  }
  int mx= 0, mx2= 0;
  for (int u: adj[v]) {
   int d= dp(v, u) + 1;
   if (mx < d) mx2= mx, mx= d;
   else if (mx2 < d) mx2= d;
  }
  if (mx + mx2 == di) ans.push_back(v);
 }
 for (auto a: ans) cout << a << '\n';
 return 0;
}