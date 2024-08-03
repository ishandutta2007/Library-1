// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1326
#include <iostream>
#include "src/Graph/Graph.hpp"
#include "src/Graph/block_cut_tree.hpp"
#include "src/Graph/HeavyLightDecomposition.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 Graph g(N, M);
 for (int i= 0; i < M; ++i) cin >> g[i], --g[i];
 HeavyLightDecomposition bct(block_cut_tree(g));
 int Q;
 cin >> Q;
 while (Q--) {
  int x, y;
  cin >> x >> y;
  if (x == y) cout << 0 << '\n';
  else cout << bct.dist(--x, --y) / 2 - 1 << '\n';
 }
 return 0;
}