// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc337/tasks/abc337_g
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/DataStructure/WaveletMatrix.hpp"
#include "src/Graph/Graph.hpp"
#include "src/Graph/HeavyLightDecomposition.hpp"
#include "src/Graph/Rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Graph g(N, N - 1);
 for (auto &e: g) cin >> e, --e;
 HeavyLightDecomposition hld(g);
 vector<int> a(N);
 for (int v= N; v--;) a[hld.to_seq(v)]= v;
 WaveletMatrix wm(a);
 auto put_edge= [&](int v, int e, long long d) -> long long {
  int u= g[e].to(v);
  if (u == hld.parent(v)) {
   auto [l, r]= hld.subtree(v);
   return d + u - wm.count(l, r, u);
  } else {
   auto [l, r]= hld.subtree(u);
   return d + wm.count(l, r, u);
  }
 };
 auto op= [&](long long l, long long r) { return l + r; };
 auto put_vertex= [&](int, long long d) { return d; };
 Rerooting<long long> dp(g, hld, put_edge, op, 0ll, put_vertex);
 for (int i= 0; i < N; ++i) cout << dp[i] + i << " \n"[i == N - 1];
 return 0;
}
