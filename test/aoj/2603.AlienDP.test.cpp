// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/2603
// Alien DP
#include <iostream>
#include <algorithm>
#include "src/Optimization/simplified_larsch_dp.hpp"
#include "src/Optimization/fibonacci_search.hpp"
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

 auto f= [&](int p) {
  auto ww= [&](int i, int j) { return w(i, j) + p; };
  auto dp= simplified_larsch_dp(n, ww);
  return dp[n] - p * m;
 };
 auto [_, ans]= fibonacci_search<MAXIMIZE>(f, -3e5, 3e5);
 cout << ans << '\n';
 return 0;
}