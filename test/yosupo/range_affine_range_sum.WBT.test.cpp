#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum"
//遅延伝搬のverify
#include <bits/stdc++.h>

#include "src/DataStructure/WeightBalancedTree.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;

using Mint = ModInt<998244353>;
struct RaffineQ_RsumQ {
  using T = Mint;
  using E = pair<Mint, Mint>;
  static T op(const T &l, const T &r) { return l + r; }
  static T mapping(const T &l, const E &r, std::size_t sz) {
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
  Mint v[N];
  for (int i = 0; i < N; i++) cin >> v[i];
  using WBT = WeightBalancedTree<RaffineQ_RsumQ>;
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
    if (WBT::percentage_used() > 97) wbt.rebuild();
  }
  return 0;
}