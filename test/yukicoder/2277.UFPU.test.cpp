// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/2277
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// ポテンシャルUF(2SAT+連結成分)
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/DataStructure/UnionFind_Potentialized_Undoable.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N, Q;
 cin >> N >> Q;
 UnionFind_Potentialized_Undoable<bool> uf(N);
 bool isok= true;
 for (int i= 0; i < Q; ++i) {
  int A, B, C;
  cin >> A >> B >> C, --A, --B;
  isok&= uf.unite(A, B, C);
 }
 if (!isok) return cout << 0 << '\n', 0;
 int cnt= 0;
 for (int i= 0; i < N; ++i) cnt+= uf.leader(i) == i;
 cout << Mint(2).pow(cnt) << '\n';
 return 0;
}