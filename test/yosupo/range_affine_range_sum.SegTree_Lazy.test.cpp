#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum"
#include <bits/stdc++.h>

#include "src/DataStructure/SegmentTree_Beats.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;

using Mint = StaticModInt<998244353>;
// RsumQはモノイドでサイズを持っておく
struct RaffineQ_RsumQ {
  struct T {
    Mint val;
    int sz;
  };
  using E = pair<Mint, Mint>;
  static T ti() { return {0, 0}; }
  static T op(const T &l, const T &r) { return {l.val + r.val, l.sz + r.sz}; }
  static bool mapping(T &v, const E &f) {
    return v.val = f.first * v.val + f.second * v.sz, true;
  }
  static void composition(E &pre, const E &suf) {
    pre = {pre.first * suf.first, suf.first * pre.second + suf.second};
  }
};

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, Q;
  cin >> N >> Q;
  vector<RaffineQ_RsumQ::T> v(N);
  for (int i = 0; i < N; i++) {
    Mint a;
    cin >> a;
    v[i] = {a, 1};
  }
  SegmentTree_Beats<RaffineQ_RsumQ> seg(v);
  while (Q--) {
    bool op;
    int l, r;
    cin >> op >> l >> r;
    if (op) {
      cout << seg.fold(l, r).val << endl;
    } else {
      Mint b, c;
      cin >> b >> c;
      seg.apply(l, r, {Mint(b), Mint(c)});
    }
  }
  return 0;
}
