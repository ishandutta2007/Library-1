#define PROBLEM "https://judge.yosupo.jp/problem/lca"
#include <iostream>
#include "src/Graph/Tree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 Tree tree(N);
 for (int i= 1; i < N; i++) {
  int p;
  cin >> p;
  tree.add_edge(i, p);
 }
 tree.build(0);
 while (Q--) {
  int u, v;
  cin >> u >> v;
  cout << tree.lca(u, v) << '\n';
 }
 return 0;
}
