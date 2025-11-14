// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1867
// competitive-verifier: TLE 2
// competitive-verifier: MLE 256
// anti monge
#include <iostream>
#include <algorithm>
#include <vector>
#include "src/Optimization/LiChaoTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N;
 cin >> N;
 int P[N];
 for (int i= 0; i < N; ++i) cin >> P[i];
 static constexpr int INF= 1e8;
 vector D(N, vector(N + 1, INF));
 for (int i= 0; i < N - 1; ++i) {
  int R[N + 1];
  fill_n(R, N + 1, 0);
  for (int j= 0; j <= i; ++j) ++R[P[j]];
  for (int j= 0; j < N; ++j) R[j + 1]+= R[j];
  int sum= 0;
  for (int j= i + 1; j < N; ++j) D[i + 1][j + 1]= sum+= R[N] - R[P[j]];
 }
 int dp[N + 1];
 fill_n(dp, N + 1, 0);
 cout << 0 << '\n';
 for (int k= 1; k < N; ++k) {
  int ndp[N + 1];
  LiChaoTree lct([&](int i, int j) { return dp[j] + D[j][i]; }, k, N + 1);
  auto tree= lct.make_tree<MINIMIZE>();
  for (int i= k; i <= N; ++i) {
   ndp[i]= tree.query(i).first;
   if (i < N) tree.insert(i, i + 1);
  }
  copy(ndp + k, ndp + N + 1, dp + k);
  cout << dp[N] << '\n';
 }
 return 0;
}