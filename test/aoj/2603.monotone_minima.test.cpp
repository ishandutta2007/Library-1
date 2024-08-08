// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/2603
// competitive-verifier: TLE 1
#include <iostream>
#include <algorithm>
#include "src/Optimization/monotone_minima.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int s, n, m;
 cin >> s >> n >> m;
 int x[s];
 for (int i= 0; i < s; ++i) cin >> x[i];
 int a[n];
 for (int i= 0; i < n; ++i) {
  int t, p;
  cin >> t >> p;
  a[i]= t - x[p - 1];
 }
 if (n <= m) return cout << 0 << '\n', 0;
 sort(a, a + n);
 int sum[n + 1];
 sum[0]= 0;
 for (int i= 0; i < n; ++i) sum[i + 1]= sum[i] + a[i];
 auto w= [&](int i, int j) { return (i - j) * a[i - 1] - (sum[i] - sum[j]); };

 int dp[n + 1];
 fill_n(dp, n + 1, 1e9);
 dp[0]= 0;
 for (int _= m; _--;) {
  auto select= [&](int i, int j, int k) { return dp[j] + w(i, j) > dp[k] + w(i, k); };
  auto id= monotone_minima(n + 1, n + 1, select);
  for (int i= n; i > 0; --i) {
   int j= id[i];
   dp[i]= dp[j] + w(i, j);
  }
 }
 cout << dp[n] << '\n';
 return 0;
}