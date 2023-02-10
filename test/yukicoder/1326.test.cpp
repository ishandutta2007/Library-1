#define PROBLEM "https://yukicoder.me/problems/no/1326"
#include <iostream>
#include "src/Graph/BiConnectedComponents.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 BiConnectedComponents bcc(N);
 for (int i= 0; i < M; ++i) {
  int u, v;
  cin >> u >> v;
  bcc.add_edge(--u, --v);
 }
 auto bct= bcc.block_cut_tree();
 bct.build();
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