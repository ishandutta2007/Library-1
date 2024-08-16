// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/2654
// competitive-verifier: TLE 2

// 永続, add_abs, rebuild の verify
#include <iostream>
#include <vector>
#include <tuple>
#include "src/Optimization/PiecewiseLinearConvex.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 vector<int> A(N + 1);
 for (int i= 0; i <= N; ++i) cin >> A[i];
 vector<vector<int>> tree(N + 1);
 for (int i= 0; i < N; ++i) {
  int u, v;
  cin >> u >> v;
  tree[u].push_back(v);
  tree[v].push_back(u);
 }
 using PLC= PiecewiseLinearConvex<int, true>;
 vector<PLC> fs(N + 1);
 vector<int> mn(N + 1), mx(N + 1);
 vector<long long> ans(N + 1);
 auto dfs= [&](auto &&dfs, int v, int p, int d) -> void {
  if (d < 2) {
   ans[v]= -1;
  } else if (mn[v] == mx[v]) {
   ans[v]= 1;
  } else {
   PLC f= fs[v], g= fs[v];
   f.add_abs(1, mn[v]);
   g.add_abs(1, mx[v]);
   ans[v]= min(f.min(), g.min());
  }
  for (int u: tree[v]) {
   if (u == p) continue;
   mn[u]= mn[v], mx[u]= mx[v], fs[u]= fs[v];
   if (A[u] < mn[u]) swap(mn[u], A[u]);
   else if (mx[u] < A[u]) swap(mx[u], A[u]);
   fs[u].add_abs(1, A[u]);
   dfs(dfs, u, v, d + 1);
  }
  if (PLC::pool_empty()) PLC::rebuild(fs);
 };
 for (int u: tree[0]) {
  tie(mn[u], mx[u])= minmax(A[0], A[u]);
  dfs(dfs, u, 0, 1);
 }
 for (int i= 1; i <= N; ++i) cout << ans[i] << "\n";
 return 0;
}