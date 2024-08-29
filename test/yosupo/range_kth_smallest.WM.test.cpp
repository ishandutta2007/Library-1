// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/range_kth_smallest
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include "src/DataStructure/WaveletMatrix.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 vector<int> a(N);
 for (int i= 0; i < N; i++) cin >> a[i];
 WaveletMatrix wm(a);
 while (Q--) {
  int l, r, k;
  cin >> l >> r >> k;
  cout << wm.kth_smallest(l, r, k) << "\n";
 }
 return 0;
}