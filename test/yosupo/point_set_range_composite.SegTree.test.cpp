#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_composite"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "DataStructure/SegmentTree.hpp"
#include "Math/ModInt.hpp"
#undef call_from_test

using Mint = ModInt<998244353>;
struct RcompositeQ {
  using T = pair<Mint, Mint>;
  static T ti() { return make_pair(Mint(1), Mint(0)); }
  static T f(const T &l, const T &r) {
    return make_pair(r.first * l.first, r.first * l.second + r.second);
  }
};

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, Q;
  cin >> N >> Q;
  vector<RcompositeQ::T> v(N);
  for (int i = 0; i < N; i++) {
    Mint a, b;
    cin >> a >> b;
    v[i] = {a, b};
  }
  SegmentTree<RcompositeQ> seg(v);
  while (Q--) {
    bool op;
    cin >> op;
    if (op) {
      int l, r;
      Mint x;
      cin >> l >> r >> x;
      auto ans = seg.query(l, r);
      cout << ans.first * x + ans.second << endl;
    } else {
      int p;
      Mint c, d;
      cin >> p >> c >> d;
      seg.set_val(p, {c, d});
    }
  }
  return 0;
}