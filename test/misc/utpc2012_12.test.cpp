// competitive-verifier: PROBLEM https://atcoder.jp/contests/utpc2012/tasks/utpc2012_12
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

#include <iostream>
#include "src/Optimization/PiecewiseLinearConvex.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N;
 cin >> N;
 vector<int> C(N);
 cin >> C[0];
 vector<vector<int>> tree(N);
 for (int i= 1, P; i < N; i++) cin >> P >> C[i], tree[--P].push_back(i);
 using PLC= PiecewiseLinearConvex<int>;
 auto dfs= [&](auto&& dfs, int v) -> PLC {
  PLC f;
  f.add_abs(1, C[v]);
  for (int u: tree[v]) {
   auto g= dfs(dfs, u);
   g.chmin_cum(), g.shift(1);
   f+= g;
  }
  return f;
 };
 cout << dfs(dfs, 0).min().value() << '\n';
 return 0;
}