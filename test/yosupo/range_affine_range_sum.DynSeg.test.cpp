#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum"

//遅延伝搬のverify

#include <bits/stdc++.h>
#include "src/DataStructure/DynamicSegmentTree.hpp"
#include "src/Math/ModInt.hpp"

using namespace std;

using Mint = ModInt<998244353>;
struct RaffineQ_RsumQ {
  using T = Mint;
  using E = pair<Mint, Mint>;
  static T ti() { return 0; }
  static T op(const T &l, const T &r) { return l + r; }
  static T mapping(const T &l, const E &r, int sz) {
    return r.first * l + r.second * sz;
  }
  static E composition(const E &l, const E &r) {
    return make_pair(r.first * l.first, r.first * l.second + r.second);
  }
};

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, Q;
  cin >> N >> Q;
  Mint a[N];
  for (int i = 0; i < N; i++) cin >> a[i];
  DynamicSegmentTree<RaffineQ_RsumQ> seg(a, a + N);
  while (Q--) {
    bool op;
    int l, r;
    cin >> op >> l >> r;
    if (op) {
      cout << seg.fold(l, r) << endl;
    } else {
      Mint b, c;
      cin >> b >> c;
      seg.apply(l, r, {Mint(b), Mint(c)});
    }
  }
  return 0;
}
