#define PROBLEM "https://yukicoder.me/problems/no/1326"
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
 int N, M;
 cin >> N >> M;
 BiConnectedComponents bcc(N);
 for (int i= 0; i < M; ++i) {
  int u, v;
  cin >> u >> v;
  bcc.add_edge(--u, --v);
 }
 auto bct= bcc.block_cut_tree();
 int K= bct.size();
 LinkCutTree<RSQ> lct(K);
 for (int v= 0; v < N; ++v) {
  for (int u: bct[v]) lct.link(u, v);
  lct.set(v, 1);
 }
 int Q;
 cin >> Q;
 while (Q--) {
  int x, y;
  cin >> x >> y;
  if (x == y) cout << 0 << '\n';
  else cout << lct.fold(--x, --y) - 2 << '\n';
 }
 return 0;
}