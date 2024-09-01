// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum
// competitive-verifier: TLE 2
// competitive-verifier: MLE 128
// insert, erase, reverse, apply, prod の verify

#include <iostream>
#include <array>
#include "src/DataStructure/RandomizedBinarySearchTree.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;

using Mint= ModInt<998244353>;
struct RaffineRsumQ {
 using T= Mint;
 using E= array<Mint, 2>;
 static T op(T vl, T vr) { return vl + vr; }
 static void mp(T &val, const E &f, int sz) { val= f[0] * val + f[1] * sz; }
 static void cp(E &pre, const E &suf) { pre[0]*= suf[0], pre[1]= suf[0] * pre[1] + suf[1]; }
 using commute= void;
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 Mint a[N];
 for (int i= 0; i < N; i++) cin >> a[i];
 RandomizedBinarySearchTree<RaffineRsumQ, true> rbst(a, a + N);
 for (int q= 0; q < Q; q++) {
  int op;
  cin >> op;
  if (op == 0) {
   int i, x;
   cin >> i >> x;
   rbst.insert(i, x);
  } else if (op == 1) {
   int i;
   cin >> i;
   rbst.erase(i);
  } else if (op == 2) {
   int l, r;
   cin >> l >> r;
   rbst.reverse(l, r);
  } else if (op == 3) {
   int l, r, b, c;
   cin >> l >> r >> b >> c;
   rbst.apply(l, r, {b, c});
  } else {
   int l, r;
   cin >> l >> r;
   cout << rbst.prod(l, r) << '\n';
  }
 }
 return 0;
}