// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/1163
// 二部マッチング (分割+分割)
#include <iostream>
#include <vector>
#include <numeric>
#include "src/Optimization/matroid_intersection.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 for (int m, n; cin >> m >> n && m;) {
  int b[m], r[n];
  for (int i= 0; i < m; ++i) cin >> b[i];
  for (int i= 0; i < n; ++i) cin >> r[i];
  vector<vector<int>> partl(m), partr(n);
  int e= 0;
  for (int i= 0; i < m; ++i)
   for (int j= 0; j < n; ++j)
    if (gcd(b[i], r[j]) > 1) partl[i].push_back(e), partr[j].push_back(e), ++e;
  PartitionMatroid M1(e, partl, vector(m, 1)), M2(e, partr, vector(n, 1));
  auto S= matroid_intersection(e, M1, M2);
  cout << S.size() << '\n';
 }
 return 0;
}