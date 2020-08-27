#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2674"

// range_freq

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/DataStructure/WaveletMatrix.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int D;
  cin >> D;
  vector<int> x(D);
  for (int i = 0; i < D; i++) cin >> x[i];
  vector<int> v(x);
  sort(v.begin(), v.end());
  v.erase(unique(v.begin(), v.end()), v.end());
  auto id
      = [&](int a) { return lower_bound(v.begin(), v.end(), a) - v.begin(); };
  vector<int> xx(D);
  for (int i = 0; i < D; i++) xx[i] = id(x[i]);
  WaveletMatrix wm(xx);
  int Q;
  cin >> Q;
  while (Q--) {
    int l, r, e;
    cin >> l >> r >> e;
    l--, r--;
    int a = id(min(x[l], x[r]) - e), b = id(max(x[l], x[r]) + e + 1);
    cout << r - l + 1 - wm.range_freq(l, r + 1, a, b) << "\n";
  }
  return 0;
}