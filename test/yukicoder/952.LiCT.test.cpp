// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/952
#include <iostream>
#include <numeric>
#include "src/Optimization/LiChaoTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N;
 cin >> N;
 int A[N];
 for (int i= 0; i < N; ++i) cin >> A[i];
 long long S[N + 1], dp[N + 2], ans[N];
 S[0]= 0;
 for (int i= 0; i < N; ++i) S[i + 1]= S[i] + A[i];
 static constexpr long long INF= 1e18;
 fill_n(dp, N + 2, INF), dp[0]= 0;
 auto w= [&](int i, int j) { return (S[i] - S[j]) * (S[i] - S[j]); };
 for (int l= N; l--;) {
  auto f= [&](int i, int j) { return dp[j] + w(i - 1, j); };
  LiChaoTree lct(f, 1, N + 2);
  auto tree= lct.make_tree<MINIMIZE>();
  long long ndp[N + 2];
  for (int i= 0; i < N + 2; ++i) {
   tree.insert(i);
   ndp[i]= tree.query(i).first;
  }
  ndp[0]= INF;
  copy_n(ndp, N + 2, dp);
  ans[l]= dp[N + 1];
 }
 for (int i= 0; i < N; ++i) cout << ans[i] << '\n';
 return 0;
}