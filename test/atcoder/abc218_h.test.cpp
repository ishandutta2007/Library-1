// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc218/tasks/abc218_h
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// Alien DP
#include <iostream>
#include <algorithm>
#include "src/Optimization/fibonacci_search.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, R;
 cin >> N >> R;
 if (R > N - R) R= N - R;
 long long A[N - 1];
 for (int i= 0; i < N - 1; ++i) cin >> A[i];
 long long B[N];
 fill_n(B, N, 0);
 for (int i= 0; i < N - 1; ++i) B[i]+= A[i], B[i + 1]+= A[i];
 auto f= [&](long long p) {
  long long dp[N + 1][2];
  dp[0][0]= 0, dp[0][1]= -1e18;
  for (int i= 0; i < N; ++i) {
   dp[i + 1][1]= dp[i][0] + B[i] - p;
   dp[i + 1][0]= max(dp[i][0], dp[i][1]);
  }
  return max(dp[N][0], dp[N][1]) + p * R;
 };
 auto a= *max_element(B, B + N);
 auto [_, ans]= fibonacci_search<MINIMIZE>(f, -3 * a, 3 * a);
 cout << ans << '\n';
 return 0;
}