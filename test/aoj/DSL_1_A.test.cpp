// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/1/DSL_1_A
#include <iostream>
#include "src/DataStructure/UnionFind.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int n, q;
 cin >> n >> q;
 UnionFind uf(n);
 for (int i= 0; i < q; i++) {
  int c, x, y;
  cin >> c >> x >> y;
  if (c) cout << uf.same(x, y) << "\n";
  else uf.unite(x, y);
 }
 return 0;
}