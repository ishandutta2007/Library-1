#define PROBLEM \
  "https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum"

// set_balance しないと スタックオーバーフロー
// （テストケース wrong_avl_killer_01）

#include <bits/stdc++.h>
#include "src/DataStructure/SplayTree.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;

using Mint = ModInt<998244353>;
struct RaffineRsumQ {
  using T = Mint;
  using E = pair<T, T>;
  static T op(const T &vl, const T &vr) { return vl + vr; }
  static void mapping(T &val, const E &f, int sz) {
    val = f.first * val + f.second * sz;
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
  Mint a[N];
  for (int i = 0; i < N; i++) cin >> a[i];
  SplayTree<RaffineRsumQ, true> splay(a, a + N);
  for (int q = 0; q < Q; q++) {
    int op;
    cin >> op;
    if (op == 0) {
      int i, x;
      cin >> i >> x;
      splay.insert(i, x);
    } else if (op == 1) {
      int i;
      cin >> i;
      splay.erase(i);
    } else if (op == 2) {
      int l, r;
      cin >> l >> r;
      splay.reverse(l, r);
    } else if (op == 3) {
      int l, r, b, c;
      cin >> l >> r >> b >> c;
      splay.apply(l, r, {b, c});
    } else {
      int l, r;
      cin >> l >> r;
      cout << splay.fold(l, r) << '\n';
    }
    if (q % 100000 == 0) splay.set_balance();
  }
  return 0;
}