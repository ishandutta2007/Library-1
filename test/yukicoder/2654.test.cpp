// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/2654
// competitive-verifier: TLE 1
#include <iostream>
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
 PiecewiseLinearConvex<int> f;
 vector<long long> ans(N + 1);
 auto dfs= [&](auto &&dfs, int v, int p, int d, int mn, int mx) -> void {
  if (d < 2) ans[v]= -1;
  else if (mn == mx) {
   ans[v]= 1;
  } else {
   f.add_abs(1, mn);
   ans[v]= f.min().value();
   f.subtract_abs(1, mn);
   f.add_abs(1, mx);
   ans[v]= min(ans[v], f.min().value());
   f.subtract_abs(1, mx);
  }
  for (int u: tree[v]) {
   if (u == p) continue;
   int mn_= mn, mx_= mx;
   int md_= A[u];
   if (md_ < mn_) swap(md_, mn_);
   else if (md_ > mx_) swap(md_, mx_);
   f.add_abs(1, md_);
   dfs(dfs, u, v, d + 1, mn_, mx_);
   f.subtract_abs(1, md_);
  }
 };
 for (int u: tree[0]) {
  auto [mn, mx]= minmax(A[u], A[0]);
  dfs(dfs, u, 0, 1, mn, mx);
 }
 for (int i= 1; i <= N; ++i) cout << ans[i] << '\n';
 return 0;
}