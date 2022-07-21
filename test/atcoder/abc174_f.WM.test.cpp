#define PROBLEM "https://atcoder.jp/contests/abc174/tasks/abc174_f"
#include <bits/stdc++.h>
#include "src/DataStructure/WaveletMatrix.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, Q;
  cin >> N >> Q;
  vector<int> c(N);
  for (int i = 0; i < N; i++) cin >> c[i];
  DQuery dq(c);
  while (Q--) {
    int l, r;
    cin >> l >> r, l--;
    cout << dq.number_of_types(l, r) << '\n';
  }
  return 0;
}