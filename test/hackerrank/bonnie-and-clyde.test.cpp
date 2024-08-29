// competitive-verifier: PROBLEM https://www.hackerrank.com/contests/w33/challenges/bonnie-and-clyde
// competitive-verifier: TLE 0.5
#include <iostream>
#include "src/Graph/Graph.hpp"
#include "src/Graph/block_cut_tree.hpp"
#include "src/Graph/HeavyLightDecomposition.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int n, m, q;
 cin >> n >> m >> q;
 Graph g(n, m);
 for (int i= 0; i < m; ++i) cin >> g[i], --g[i];
 HeavyLightDecomposition bct(block_cut_tree(g));
 while (q--) {
  int u, v, w;
  cin >> u >> v >> w;
  --u, --v, --w;
  if (!bct.connected(u, w) || !bct.connected(w, v)) cout << "NO";
  else {
   int tmp= bct.dist(u, w) + bct.dist(w, v) - bct.dist(u, v);
   cout << (tmp == 0 || tmp == 2 ? "YES" : "NO");
  }
  if (q) cout << '\n';
 }
 return 0;
}