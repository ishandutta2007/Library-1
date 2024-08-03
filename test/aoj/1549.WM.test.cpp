// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/UOA/UAPC/1549
#include <iostream>
#include <vector>
#include "src/DataStructure/WaveletMatrix.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 vector<int> a(N);
 for (int i= 0; i < N; i++) cin >> a[i];
 WaveletMatrix wm(a);
 int Q;
 cin >> Q;
 while (Q--) {
  int l, r, d;
  cin >> l >> r >> d, r++;
  int cnt= wm.count(l, r, d);
  if (cnt == 0) cout << wm.kth_smallest(l, r, 0) - d << '\n';
  else if (cnt == r - l) {
   cout << d - wm.kth_largest(l, r, 0) << '\n';
  } else {
   cout << min(wm.kth_smallest(l, r, cnt) - d, d - wm.kth_smallest(l, r, cnt - 1)) << '\n';
  }
 }
 return 0;
}