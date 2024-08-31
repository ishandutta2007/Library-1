// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1833
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include <algorithm>
#include "src/Math/ModInt.hpp"
#include "src/Graph/Graph.hpp"
#include "src/Graph/Rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Graph g(N, N - 1);
 int d= 0;
 vector<int> c(N);
 for (int i= 0; i < N - 1; ++i) cin >> g[i] >> c[i], --g[i], d= max(d, c[i]);
 using Dat= array<int, 3>;
 auto put_edge= [&](int, int e, Dat dp) {
  dp[0]= max(dp[0], c[e]);
  dp[1]= max(dp[1], d - c[e]);
  dp[2]= min(max(dp[2], c[e]), dp[1]);
  return dp;
 };
 auto op= [&](const Dat &l, const Dat &r) {
  Dat ret;
  ret[0]= max(l[0], r[0]);
  ret[1]= min(max(l[1], r[0]), max(l[0], r[1]));
  ret[2]= min({max(l[2], r[0]), max(l[0], r[2]), max(l[1], r[1])});
  return ret;
 };
 auto put_vertex= [&](int, Dat dp) {
  dp[1]= min(dp[1], dp[0]);
  dp[2]= min(dp[2], dp[1]);
  return dp;
 };
 int ans= d;
 for (auto [_, __, x]: Rerooting<Dat>(g, put_edge, op, Dat{0, d, d}, put_vertex)) ans= min(ans, x);
 cout << ans << "\n";
 return 0;
}