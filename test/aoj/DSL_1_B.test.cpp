// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/1/DSL_1_B
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

// ポテンシャルUF
#include <iostream>
#include "src/DataStructure/UnionFind_Potentialized.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int n, q;
 cin >> n >> q;
 UnionFind_Potentialized<int> uf(n);
 for (int i= 0; i < q; i++) {
  int c, x, y;
  cin >> c >> x >> y;
  if (c) {
   if (uf.connected(x, y)) cout << uf.diff(x, y) << "\n";
   else cout << "?\n";
  } else {
   int z;
   cin >> z;
   uf.unite(x, y, z);
  }
 }
 return 0;
}