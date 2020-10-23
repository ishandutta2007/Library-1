#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/2/DSL_2_D"
#include <bits/stdc++.h>
#include "src/DataStructure/SegmentTree_Dual.hpp"
using namespace std;

struct RupdQ {
  using T = int;
  using E = int;
  static E ei() { return INT_MAX; }
  static T g(const T& l, const E& r) { return r == ei() ? l : r; }
  static E h(const E& l, const E& r) { return r == ei() ? l : r; }
};

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int n, q;
  cin >> n >> q;
  SegmentTree_Dual<RupdQ> seg(n, INT_MAX);
  while (q--) {
    bool com;
    cin >> com;
    if (com) {
      int i;
      cin >> i;
      cout << seg[i] << endl;
    } else {
      int s, t, x;
      cin >> s >> t >> x;
      seg.update(s, t + 1, x);
    }
  }
  return 0;
}