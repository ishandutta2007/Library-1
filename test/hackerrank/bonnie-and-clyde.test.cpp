#define PROBLEM "https://www.hackerrank.com/contests/w33/challenges/bonnie-and-clyde"
#include <iostream>
#include "src/Graph/BiConnectedComponents.hpp"
using namespace std;
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