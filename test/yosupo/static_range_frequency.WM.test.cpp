#define PROBLEM "https://judge.yosupo.jp/problem/static_range_frequency"
#include <bits/stdc++.h>
#include "src/DataStructure/WaveletMatrix.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, Q;
  cin >> N >> Q;
  if (!N)
    while (Q--) {
      int l, r, x;
      cin >> l >> r >> x;
      cout << 0 << '\n';
    }
  else {
    vector<int> a(N);
    for (int i = 0; i < N; i++) cin >> a[i];
    WaveletMatrix wm(a);
    while (Q--) {
      int l, r, x;
      cin >> l >> r >> x;
      cout << wm.rank(x, r) - wm.rank(x, l) << "\n";
    }
  }
  return 0;
}