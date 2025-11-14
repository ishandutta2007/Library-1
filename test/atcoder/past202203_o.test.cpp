// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/past202203-open/tasks/past202203_o
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// ポテンシャルUF (2色塗り分け+連結成分)
#include <iostream>
#include <algorithm>
#include "src/DataStructure/UnionFind_Potentialized.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 UnionFind_Potentialized<bool> uf(N);
 bool odd[N];
 fill_n(odd, N, 0);
 for (int i= 0; i < M; ++i) {
  int A, B;
  cin >> A >> B, --A, --B;
  int x= uf.leader(A), y= uf.leader(B);
  odd[x]= odd[y]= odd[x] | odd[y];
  if (!uf.unite(A, B, 1)) odd[uf.leader(A)]= 1;
 }
 int cnt[N];
 fill_n(cnt, N, 0);
 for (int i= 0; i < N; ++i)
  if (uf.potential(i)) ++cnt[uf.leader(i)];
 int Z= N / 3, X= (N + 2) / 3, Y= (N + 1) / 3;
 bool dp[X + 1][Y + 1];
 for (int i= X + 1; i--;)
  for (int j= Y + 1; j--;) dp[i][j]= 0;
 dp[0][0]= 1;
 for (int i= 0; i < N; ++i) {
  if (uf.leader(i) != i) continue;
  if (odd[i]) {
   Z-= uf.size(i);
   if (Z < 0) return cout << "No" << '\n', 0;
   continue;
  }
  int a= cnt[i], b= uf.size(i) - cnt[i];
  for (int j= X + 1; j--;)
   for (int k= Y + 1; k--;) {
    if (j >= a && k >= b) dp[j][k]|= dp[j - a][k - b];
    if (j >= b && k >= a) dp[j][k]|= dp[j - b][k - a];
   }
 }
 cout << (dp[X][Y] ? "Yes" : "No") << '\n';
 return 0;
}