#define PROBLEM "https://judge.yosupo.jp/problem/range_kth_smallest"
#include <bits/stdc++.h>
#include "src/DataStructure/WaveletMatrix.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, Q;
  cin >> N >> Q;
  vector<int> a(N);
  for (int i = 0; i < N; i++) cin >> a[i];
  vector<int> v(a);
  sort(v.begin(), v.end());
  v.erase(unique(v.begin(), v.end()), v.end());
  for (int i = 0; i < N; i++)
    a[i] = lower_bound(v.begin(), v.end(), a[i]) - v.begin();
  WaveletMatrix wm(a);
  while (Q--) {
    int l, r, k;
    cin >> l >> r >> k;
    cout << v[wm.kth_smallest(l, r, k)] << "\n";
  }
  return 0;
}