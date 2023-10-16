#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2725"
#include <iostream>
#include <numeric>
#include "src/Optimization/LiChaoTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, T;
 cin >> N >> T;
 int t[N], p[N], f[N];
 for (int i= 0; i < N; ++i) cin >> t[i] >> p[i] >> f[i];
 int ord[N];
 iota(ord, ord + N, 0), sort(ord, ord + N, [&](int i, int j) { return f[i] < f[j]; });
 LiChaoTree lct([](int x, int a, long long b) {
  long long t= x - a;
  return b - t * t;
 });
 vector lcts(T + 1, lct.make_tree<MAXIMIZE>());
 long long ans= -1e9;
 for (int i= 0; i < N; ++i) {
  int I= ord[i], ti= t[I];
  for (int x= T; x >= ti; --x) {
   auto [val, _]= lcts[x - ti].query(f[I]);
   val= max(0ll, val) + p[I];
   ans= max(ans, val);
   lcts[x].insert(f[I], val);
  }
 }
 cout << ans << '\n';
 return 0;
}