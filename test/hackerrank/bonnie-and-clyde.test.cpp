#define PROBLEM "https://www.hackerrank.com/contests/w33/challenges/bonnie-and-clyde"
#include <iostream>
#include "src/Graph/BiConnectedComponents.hpp"
#include "src/DataStructure/LinkCutTree.hpp"
using namespace std;
struct RSQ {
 using T= int;
 static T op(T l, T r) { return l + r; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int n, m, q;
 cin >> n >> m >> q;
 BiConnectedComponents bcc(n);
 for (int i= 0; i < m; ++i) {
  int u, v;
  cin >> u >> v;
  bcc.add_edge(--u, --v);
 }
 auto bct= bcc.block_cut_tree();
 int N= bct.size();
 LinkCutTree<RSQ> lct(N, 1);
 for (int v= 0; v < n; ++v)
  for (int u: bct[v]) lct.link(u, v);
 while (q--) {
  int u, v, w;
  cin >> u >> v >> w;
  --u, --v, --w;
  if (lct.lca(u, w) == -1 || lct.lca(w, v) == -1) cout << "NO";
  else {
   int tmp= lct.fold(u, w) + lct.fold(w, v) - lct.fold(u, v);
   cout << (tmp == 1 || tmp == 3 ? "YES" : "NO");
  }
  if (q) cout << '\n';
 }
 return 0;
}