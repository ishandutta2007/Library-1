// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/unionfind_with_potential
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/DataStructure/UnionFind_Potentialized_Undoable.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 int N, Q;
 cin >> N >> Q;
 UnionFind_Potentialized_Undoable<Mint> uf(N);
 while (Q--) {
  int t, u, v;
  cin >> t >> u >> v;
  if (t) {
   if (uf.connected(u, v)) cout << uf.diff(u, v) << '\n';
   else cout << -1 << '\n';
  } else {
   Mint x;
   cin >> x;
   cout << uf.unite(u, v, x) << '\n';
  }
 }
 return 0;
}