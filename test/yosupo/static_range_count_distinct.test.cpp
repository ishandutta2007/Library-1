// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/static_range_count_distinct
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/DataStructure/RangeCountDistinct.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 vector<int> a(N);
 for (int i= 0; i < N; ++i) cin >> a[i];
 RangeCountDistinct rcd(a);
 while (Q--) {
  int l, r;
  cin >> l >> r;
  cout << rcd.count(l, r) << '\n';
 }
 return 0;
}
