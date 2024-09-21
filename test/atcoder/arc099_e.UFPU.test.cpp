// competitive-verifier: PROBLEM https://atcoder.jp/contests/arc099/tasks/arc099_e
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// https://atcoder.jp/contests/arc099/tasks/arc099_c
// 2色塗り分け+連結成分
#include <iostream>
#include <algorithm>
#include <bitset>
#include "src/DataStructure/UnionFind_Potentialized_Undoable.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 bool adj[N][N];
 for (int i= N; i--;)
  for (int j= i; j--;) adj[i][j]= adj[j][i]= 1;
 for (int i= M; i--;) {
  int A, B;
  cin >> A >> B, --A, --B;
  adj[A][B]= adj[B][A]= 0;
 }
 UnionFind_Potentialized_Undoable<bool> uf(N);
 for (int i= N; i--;)
  for (int j= i; j--;)
   if (adj[i][j])
    if (!uf.unite(i, j, 1)) return cout << "-1\n", 0;
 int cnt[N];
 fill_n(cnt, N, 0);
 for (int i= N; i--;)
  if (uf.potential(i)) ++cnt[uf.leader(i)];
 bitset<701> dp;
 dp[0]= 1;
 for (int i= N; i--;) {
  if (uf.leader(i) != i) continue;
  dp= (dp << cnt[i]) | (dp << (uf.size(i) - cnt[i]));
 }
 int ans= N * N;
 for (int i= N + 1; i--;)
  if (dp[i]) ans= min(ans, i * (i - 1) / 2 + (N - i) * (N - i - 1) / 2);
 cout << ans << '\n';
 return 0;
}