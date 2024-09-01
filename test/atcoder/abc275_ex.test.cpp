// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc275/tasks/abc275_Ex
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
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
 using PLC= PiecewiseLinearConvex<int>;
 auto dfs= [&](auto &&dfs, int v) -> PLC {
  PLC f;
  for (int u: ct.children(v))
   if (u != -1) f+= dfs(dfs, u);
  f.add_linear(B[v]);
  f.add_inf(false, A[v]);
  f.chmin_cum(true);
  f.add_linear(-B[v]);
  return f;
 };
 cout << dfs(dfs, root)(0).value() << '\n';
 return 0;
}