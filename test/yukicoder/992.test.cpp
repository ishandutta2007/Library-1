// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/992
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Misc/longest_increasing_subsequence.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<int(1e9 + 7)>;
 int N;
 cin >> N;
 vector<int> A(N);
 for (int i= 0; i < N; ++i) cin >> A[i];
 auto [_, cand]= longest_increasing_subsequence(A);
 int k= cand.size();
 vector<Mint> dp(cand[0].size(), 1);
 for (int i= 1; i < k; ++i) {
  int n= cand[i - 1].size(), m= cand[i].size();
  vector<Mint> ndp(m);
  Mint sum= 0;
  for (int j= 0, l= 0, r= 0; j < m; ++j) {
   while (r < n && cand[i - 1][r] < cand[i][j]) sum+= dp[r++];
   while (l < n && A[cand[i - 1][l]] >= A[cand[i][j]]) sum-= dp[l++];
   ndp[j]= sum;
  }
  dp= std::move(ndp);
 }
 Mint ans= 0;
 for (auto x: dp) ans+= x;
 cout << ans << '\n';
 return 0;
}