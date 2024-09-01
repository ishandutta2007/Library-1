// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/lca
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Graph/Graph.hpp"
#include "src/Graph/HeavyLightDecomposition.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 Graph g(N);
 for (int i= 1, p; i < N; ++i) cin >> p, g.add_edge(p, i);
 HeavyLightDecomposition tree(g, 0);
 while (Q--) {
  int u, v;
  cin >> u >> v;
  cout << tree.lca(u, v) << '\n';
 }
 return 0;
}
