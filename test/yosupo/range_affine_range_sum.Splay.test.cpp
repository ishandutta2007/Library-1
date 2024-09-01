// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/range_affine_range_sum
// competitive-verifier: TLE 2
// competitive-verifier: MLE 64
// apply, prod の verify

#include <iostream>
#include "src/DataStructure/SplayTree.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;

using Mint= ModInt<998244353>;
struct RaffineQ_RsumQ {
 using T= Mint;
 using E= pair<Mint, Mint>;
 static T op(const T &l, const T &r) { return l + r; }
 static void mp(T &v, const E &f, std::size_t sz) { v= f.first * v + f.second * sz; }
 static void cp(E &pre, const E &suf) { pre= {suf.first * pre.first, suf.first * pre.second + suf.second}; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 Mint v[N];
 for (int i= 0; i < N; i++) cin >> v[i];
 SplayTree<RaffineQ_RsumQ> st(v, v + N);
 while (Q--) {
  bool op;
  int l, r;
  cin >> op >> l >> r;
  if (op) {
   cout << st.prod(l, r) << '\n';
  } else {
   Mint b, c;
   cin >> b >> c;
   st.apply(l, r, {b, c});
  }
 }
 return 0;
}