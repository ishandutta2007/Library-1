#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/2/DSL_2_F"
#include <bits/stdc++.h>
#include "src/DataStructure/SegmentTree_Beats.hpp"
using namespace std;

struct RupdQ_RminQ {
  using T = int;
  using E = int;
  static T ti() { return INT_MAX; }
  static T op(const T& l, const T& r) { return min(l, r); }
  static bool mapping(T& v, const E& f) { return v = f, true; }
  static void composition(E& pre, const E& suf) { pre = suf; }
};

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int n, q;
  cin >> n >> q;
  SegmentTree_Beats<RupdQ_RminQ> seg(n);
  while (q--) {
    int com, s, t;
    cin >> com >> s >> t;
    if (com) {
      cout << seg.fold(s, t + 1) << endl;
    } else {
      int x;
      cin >> x;
      seg.apply(s, t + 1, x);
    }
  }
  return 0;
}
