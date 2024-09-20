// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/unionfind
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/DataStructure/UnionFind.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, Q;
 cin >> N >> Q;
 UnionFind uf(N);
 while (Q--) {
  int t, u, v;
  cin >> t >> u >> v;
  if (t) cout << uf.connected(u, v) << '\n';
  else uf.unite(u, v);
 }
 return 0;
}