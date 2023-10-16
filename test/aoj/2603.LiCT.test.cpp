#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2603"
#include <iostream>
#include <algorithm>
#include "src/Optimization/LiChaoTree.hpp"
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
  LiChaoTree lct([&](int i, int j) { return dp[j] + w(i, j); }, 1, n + 1);
  auto tree= lct.make_tree<MINIMIZE>();
  int ndp[n + 1];
  tree.insert(0), ndp[0]= 0;
  for (int i= 1; i <= n; ++i) {
   ndp[i]= tree.query(i).first;
   tree.insert(i);
  }
  copy_n(ndp, n + 1, dp);
 }
 cout << dp[n] << '\n';
 return 0;
}