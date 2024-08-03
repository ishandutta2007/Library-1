// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/2725
#include <iostream>
#include <numeric>
#include "src/Optimization/ConvexHullTrick.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, T;
 cin >> N >> T;
 int t[N], p[N], f[N];
 for (int i= 0; i < N; ++i) cin >> t[i] >> p[i] >> f[i];
 int ord[N];
 iota(ord, ord + N, 0), sort(ord, ord + N, [&](int i, int j) { return f[i] < f[j]; });
 ConvexHullTrick<long long, MAXIMIZE> cht[T + 1];
 long long ans= -1e9;
 for (int i= 0; i < N; ++i) {
  int I= ord[i], ti= t[I];
  for (int x= T; x >= ti; --x) {
   long long val= p[I];
   if (!cht[x - ti].empty()) val= max(val, cht[x - ti].query(f[I]) + p[I] - f[I] * f[I]);
   ans= max(ans, val);
   cht[x].insert(2 * f[I], val - f[I] * f[I]);
  }
 }
 cout << ans << '\n';
 return 0;
}