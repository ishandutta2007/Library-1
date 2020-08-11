#define PROBLEM \
  "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_A"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/DataStructure/SegmentTree.hpp"
#undef call_from_test

struct RminQ {
  using T = int;
  static T ti() { return INT_MAX; }
  static T f(const T &l, const T &r) { return min(l, r); }
};

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int n, q;
  cin >> n >> q;
  SegmentTree<RminQ> seg(n);
  while (q--) {
    int com, x, y;
    cin >> com >> x >> y;
    if (com) {
      cout << seg.query(x, y + 1) << endl;
    } else {
      seg.set_val(x, y);
    }
  }
  return 0;
}