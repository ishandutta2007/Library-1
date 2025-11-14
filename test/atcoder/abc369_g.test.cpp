// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc369/tasks/abc369_g
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 256
#include <iostream>
#include <vector>
#include "src/Graph/Graph.hpp"
#include "src/Optimization/PiecewiseLinearConvex.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using PLC= PiecewiseLinearConvex<long long>;
 int N;
 cin >> N;
 Graph g(N, N - 1);
 vector<long long> L(N - 1);
 for (int i= 0; i < N - 1; ++i) cin >> g[i] >> L[i], --g[i], L[i]*= 2;
 auto adj= g.adjacency_edge(0);
 auto dfs= [&](auto &&dfs, int v, int p) -> PLC {
  PLC f;
  for (auto e: adj[v]) {
   int u= g[e].to(v);
   if (u == p) continue;
   PLC fu= dfs(dfs, u, v);
   fu.add_linear(-1);
   fu.chmin_slide_win(-L[e], 0);
   fu.add_linear(1);
   fu.add_const(L[e]);
   f+= fu;
  }
  f.chmin_cum(true);
  f.add_ramp(1, 0);
  return f;
 };
 PLC f= dfs(dfs, 0, -1);
 for (int i= 1; i <= N; ++i) {
  f.add_linear(-i);
  cout << (long long)f.min().value() << '\n';
  f.add_linear(i);
 }
 return 0;
}