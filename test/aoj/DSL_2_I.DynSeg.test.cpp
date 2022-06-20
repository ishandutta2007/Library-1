#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/2/DSL_2_I"

// 遅延伝搬のverify

#include <bits/stdc++.h>
#include "src/DataStructure/SegmentTree_Dynamic.hpp"

using namespace std;

struct RSQandRUQ {
  using T = int;
  using E = int;
  static T op(T l, T r) { return l + r; }
  static T ti() { return 0; }
  static void mapping(T& v, E f, int sz) { v = f * sz; }
  static void composition(E& pre, E suf) { pre = suf; }
};

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int n, q;
  cin >> n >> q;
  SegmentTree_Dynamic<RSQandRUQ> seg(n, 0);
  for (int i = 0; i < q; i++) {
    int op, s, t;
    cin >> op >> s >> t;
    if (op == 0) {
      int x;
      cin >> x;
      seg.apply(s, t + 1, x);
    } else {
      cout << seg.fold(s, t + 1) << '\n';
    }
  }
  return 0;
}
