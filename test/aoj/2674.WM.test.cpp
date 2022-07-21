#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/challenges/sources/VPC/HUPC/2674"
// range_freq
#include <bits/stdc++.h>
#include "src/DataStructure/WaveletMatrix.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int D;
  cin >> D;
  vector<int> x(D);
  for (int i = 0; i < D; i++) cin >> x[i];
  WaveletMatrix wm(x);
  int Q;
  cin >> Q;
  while (Q--) {
    int l, r, e;
    cin >> l >> r >> e, l--, r--;
    int a = min(x[l], x[r]) - e, b = max(x[l], x[r]) + e + 1;
    cout << r - l + 1 - wm.count(l, r + 1, a, b) << "\n";
  }
  return 0;
}