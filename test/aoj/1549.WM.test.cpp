#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1549"
// next_value, prev_value
#include <bits/stdc++.h>
#include "src/DataStructure/WaveletMatrix.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N;
  cin >> N;
  const int OFS = 1e6;
  vector<int> a(N);
  for (int i = 0; i < N; i++) cin >> a[i], a[i] += OFS;
  WaveletMatrix wm(a);
  int Q;
  cin >> Q;
  while (Q--) {
    int l, r, d;
    cin >> l >> r >> d;
    d += OFS;
    int ans = INT_MAX;
    int suc = wm.next_value(l, r + 1, d);
    if (suc != -1) ans = min(ans, suc - d);
    int pre = wm.prev_value(l, r + 1, d);
    if (pre != -1) ans = min(ans, d - pre);
    cout << ans << "\n";
  }
  return 0;
}