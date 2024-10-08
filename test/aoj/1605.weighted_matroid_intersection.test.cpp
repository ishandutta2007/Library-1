// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/1605
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// 重み付き
// (グラフ+分割)
#include <iostream>
#include <vector>
#include "src/Optimization/matroid_intersection.hpp"
using namespace std;
int main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 for (int n, m, k; cin >> n >> m >> k && n;) {
  GraphicMatroid M1(n);
  vector<vector<int>> parts(2);
  vector<int> w(m);
  for (int i= 0; i < m; i++) {
   int u, v;
   char l;
   cin >> u >> v >> w[i] >> l, u--, v--;
   M1.add_edge(u, v);
   parts[l == 'B'].push_back(i);
  }
  PartitionMatroid M2(m, parts, {k, n - 1 - k});
  auto S= weighted_matroid_intersection<MINIMIZE>(m, M1, M2, w);
  if (S.size() < n) cout << -1 << '\n';
  else {
   int ans= 0;
   for (int e: S[n - 1]) ans+= w[e];
   cout << ans << '\n';
  }
 }
 return 0;
}