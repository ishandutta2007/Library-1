#define PROBLEM "https://yukicoder.me/problems/no/952"
#include <iostream>
#include <numeric>
#include "src/Optimization/monotone_minima.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N;
 cin >> N;
 int A[N];
 for (int i= 0; i < N; ++i) cin >> A[i];
 long long S[N + 1]= {0}, dp[N + 2], ans[N];
 for (int i= 0; i < N; ++i) S[i + 1]= S[i] + A[i];
 static constexpr long long INF= 1e18;
 fill_n(dp, N + 2, INF), dp[0]= 0;
 auto w= [&](int i, int j) { return (S[i] - S[j]) * (S[i] - S[j]); };
 for (int l= N; l--;) {
  auto select= [&](int i, int j, int k) {
   if (i <= k) return false;
   if (i <= j) return true;
   return dp[j] + w(i - 1, j) > dp[k] + w(i - 1, k);
  };
  auto id= monotone_minima(N + 2, N + 1, select);
  for (int i= N + 2; --i;) {
   int j= id[i];
   dp[i]= dp[j] + w(i - 1, j);
  }
  dp[0]= INF;
  ans[l]= dp[N + 1];
 }
 for (int i= 0; i < N; ++i) cout << ans[i] << '\n';
 return 0;
}