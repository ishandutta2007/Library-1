// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/2293
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// ポテンシャル(無向2SAT) + rollback
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
 UnionFind_Potentialized_Undoable<bool> uf(N);
 Mint pw[N + 1];
 pw[0]= 1;
 for (int i= 1; i <= N; ++i) pw[i]= pw[i - 1] + pw[i - 1];
 int n= N;
 bool ok= true;
 while (Q--) {
  int t;
  cin >> t;
  if (t == 3) {
   uf.rollback(0);
   n= N, ok= true;
  } else {
   int u, v;
   cin >> u >> v, --u, --v;
   if (!uf.connected(u, v)) --n;
   ok&= uf.unite(u, v, t - 1);
  }
  cout << (ok ? pw[n] : 0) << '\n';
 }
 return 0;
}