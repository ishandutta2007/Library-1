// competitive-verifier: PROBLEM https://atcoder.jp/contests/s8pc_1/tasks/s8pc_1_h
// competitive-verifier: TLE 4.5
// competitive-verifier: MLE 64
// https://atcoder.jp/contests/s8pc-1/tasks/s8pc_1_h
#include <iostream>
#include <vector>
#include <array>
#include "src/DataStructure/KDTree.hpp"
using namespace std;
struct RSQ {
 using T= int;
 static T ti() { return 0; }
 static T op(T a, T b) { return a + b; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, D, E;
 cin >> N >> D >> E;
 vector<int> A(N);
 for (int i= 0; i < N; i++) cin >> A[i];
 vector<array<long long, 2>> v[2][3];
 auto dfs= [&](auto &dfs, int i, int s, int g, int e, long long x, long long y) {
  if (s == g) {
   v[i][e].push_back({x, y});
   return;
  }
  dfs(dfs, i, s + 1, g, e, x - A[s], y - A[s]);
  dfs(dfs, i, s + 1, g, e, x + A[s], y);
  dfs(dfs, i, s + 1, g, e, x, y + A[s]);
  if (e < E) dfs(dfs, i, s + 1, g, e + 1, x, y);
 };
 int n= N / 2;
 dfs(dfs, 0, 0, n, 0, 0, 0);
 dfs(dfs, 1, n, N, 0, 0, 0);
 long long ans= 0;
 for (int e= 0; e <= E; ++e) {
  KDTree<long long, 2, RSQ> kdt(v[1][e], 1);
  for (int f= 0; e + f <= E; ++f) {
   for (int i= v[0][f].size(); i--;) {
    auto [a, b]= v[0][f][i];
    ans+= kdt.prod_cuboid(-D - a, D - a, -D - b, D - b);
   }
  }
 }
 cout << ans << '\n';
 return 0;
}