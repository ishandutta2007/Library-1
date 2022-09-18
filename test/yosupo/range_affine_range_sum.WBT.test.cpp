#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum"
//遅延伝搬のverify
#include <bits/stdc++.h>

#include "src/DataStructure/WeightBalancedTree.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;

using Mint = StaticModInt<998244353>;
struct RaffineQ_RsumQ {
  using T = Mint;
  using E = pair<Mint, Mint>;
  static T op(const T &l, const T &r) { return l + r; }
  static void mapping(T &v, const E &f, std::size_t sz) {
    v = f.first * v + f.second * sz;
  }
  static void composition(E &pre, const E &suf) {
    pre = {suf.first * pre.first, suf.first * pre.second + suf.second};
  }
};

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, Q;
  cin >> N >> Q;
  Mint v[N];
  for (int i = 0; i < N; i++) cin >> v[i];
  using WBT = WeightBalancedTree<RaffineQ_RsumQ, 1 << 24>;
  WBT wbt(v, v + N);
  while (Q--) {
    bool op;
    int l, r;
    cin >> op >> l >> r;
    if (op) {
      cout << wbt.fold(l, r) << endl;
    } else {
      Mint b, c;
      cin >> b >> c;
      wbt.apply(l, r, {b, c});
    }
    if (WBT::percentage_used() > 90) wbt.rebuild();
  }
  return 0;
}