// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc174/tasks/abc174_f
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
 vector<int> c(N);
 for (int i= 0; i < N; i++) cin >> c[i];
 RangeCountDistinct dq(c);
 while (Q--) {
  int l, r;
  cin >> l >> r, l--;
  cout << dq.count(l, r) << '\n';
 }
 return 0;
}