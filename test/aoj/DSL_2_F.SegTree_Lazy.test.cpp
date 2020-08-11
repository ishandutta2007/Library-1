#define PROBLEM \
  "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_F"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/DataStructure/SegmentTree_Lazy.hpp"
#undef call_from_test

struct RupdQ_RminQ {
  using T = int;
  using E = int;
  static T ti() { return INT_MAX; }
  static E ei() { return INT_MAX; }
  static T f(const T& l, const T& r) { return min(l, r); }
  static T g(const T& l, const E& r) { return r == ei() ? l : r; }
  static E h(const E& l, const E& r) { return r == ei() ? l : r; }
};

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int n, q;
  cin >> n >> q;
  SegmentTree_Lazy<RupdQ_RminQ> seg(n);
  while (q--) {
    int com, s, t;
    cin >> com >> s >> t;
    if (com) {
      cout << seg.query(s, t + 1) << endl;
    } else {
      int x;
      cin >> x;
      seg.update(s, t + 1, x);
    }
  }
  return 0;
}
