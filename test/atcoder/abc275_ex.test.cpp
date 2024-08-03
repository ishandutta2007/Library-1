// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc275/tasks/abc275_Ex
// https://atcoder.jp/contests/abc275/tasks/abc275_h
#include <iostream>
#include <vector>
#include "src/Misc/CartesianTree.hpp"
#include "src/Optimization/PiecewiseLinearConvex.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 vector<int> A(N), B(N);
 for (int i= 0; i < N; ++i) cin >> A[i];
 for (int i= 0; i < N; ++i) cin >> B[i];
 CartesianTree ct(B, false);
 int root= ct.root();
 vector<PiecewiseLinearConvex<int>> dp(N);
 auto dfs= [&](auto self, int v) -> void {
  for (int u: ct.children(v))
   if (u != -1) {
    self(self, u);
    dp[v]+= dp[u];
   }
  dp[v].add_linear(B[v]);
  dp[v].add_inf(false, A[v]);
  dp[v].chmin_cum(true);
  dp[v].add_linear(-B[v]);
 };
 dfs(dfs, root);
 cout << dp[root](0) << '\n';
 return 0;
}