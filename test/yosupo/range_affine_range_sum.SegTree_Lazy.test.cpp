#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum"
#include <bits/stdc++.h>
#include "src/DataStructure/SegmentTree_Lazy.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;

using Mint = ModInt<998244353>;
// RsumQはモノイドでサイズを持っておく
struct RaffineQ_RsumQ {
  struct T {
    Mint val;
    int size;
    T(Mint v = 0, int s = 1) : val(v), size(s) {}
  };
  using E = pair<Mint, Mint>;
  static T ti() { return T(0, 0); }
  static E ei() { return make_pair(Mint(1), Mint(0)); }
  static T f(const T &l, const T &r) {
    return T(l.val + r.val, l.size + r.size);
  }
  static T g(const T &l, const E &r) {
    return T(r.first * l.val + r.second * l.size, l.size);
  }
  static E h(const E &l, const E &r) {
    return make_pair(r.first * l.first, r.first * l.second + r.second);
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
  SegmentTree_Lazy<RaffineQ_RsumQ> seg(v);
  while (Q--) {
    bool op;
    int l, r;
    cin >> op >> l >> r;
    if (op) {
      cout << seg.query(l, r).val << endl;
    } else {
      Mint b, c;
      cin >> b >> c;
      seg.update(l, r, {Mint(b), Mint(c)});
    }
  }
  return 0;
}
