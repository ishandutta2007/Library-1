// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/lesson/8/ITP2/all/ITP2_4_B
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// split, split3, operator+ の verify

#include <iostream>
#include "src/DataStructure/SplayTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int n;
 cin >> n;
 int a[n];
 for (int i= 0; i < n; ++i) cin >> a[i];
 SplayTree<int> ar(a, a + n);
 int q;
 cin >> q;
 for (int i= 0; i < q; ++i) {
  int b, m, e;
  cin >> b >> m >> e;
  auto [l, c, r]= ar.split3(b, e);
  auto [cl, cr]= c.split(m - b);
  ar= l + cr + cl + r;
 }
 auto ans= ar.dump();
 for (int i= 0; i < n; ++i) cout << ans[i] << " \n"[i == n - 1];
 return 0;
}