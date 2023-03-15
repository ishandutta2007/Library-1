#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2429"
// 重み付き
// 二部マッチング (100+100)
// |台集合| <= 1e4
// 構築
#include <iostream>
#include <vector>
#include <tuple>
#include "src/Optimization/matroid_intersection.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int n;
 cin >> n;
 int W[n][n], E[n][n];
 for (int i= 0; i < n; ++i)
  for (int j= 0; j < n; ++j) cin >> W[i][j];
 for (int i= 0; i < n; ++i)
  for (int j= 0; j < n; ++j) cin >> E[i][j];
 int m= n * n;
 vector<long long> w(m);
 vector<vector<int>> partl(n), partr(n);
 long long mincost= 0;
 string F[n];
 for (int i= 0; i < n; ++i) {
  cin >> F[i];
  int e= i * n;
  for (int j= 0; j < n; ++j, ++e) {
   partl[i].push_back(e), partr[j].push_back(e);
   if (F[i][j] == '.') w[e]= W[i][j];
   else w[e]= -E[i][j], mincost+= E[i][j];
  }
 }
 PartitionMatroid M1(m, partl, vector(n, 1)), M2(m, partr, vector(n, 1));
 auto S= weighted_matroid_intersection<-1>(m, M1, M2, w);
 bool write[n][n];
 for (int i= n; i--;)
  for (int j= n; j--;) write[i][j]= 0;
 for (auto d: S[n]) mincost+= w[d], write[d / n][d % n]= 1;
 cout << mincost << '\n';
 vector<tuple<int, int, string>> ans;
 for (int i= 0; i < n; ++i)
  for (int j= 0; j < n; ++j)
   if (write[i][j] ^ (F[i][j] != '.')) ans.emplace_back(i + 1, j + 1, write[i][j] ? "write" : "erase");
 cout << ans.size() << '\n';
 for (auto [R, C, operate]: ans) cout << R << " " << C << " " << operate << '\n';
 return 0;
}