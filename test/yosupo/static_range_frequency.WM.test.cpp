#define PROBLEM "https://judge.yosupo.jp/problem/static_range_frequency"
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
  int l, r, x;
  cin >> l >> r >> x;
  cout << (l < r ? wm.count(l, r, x, x + 1) : 0) << "\n";
 }
 return 0;
}