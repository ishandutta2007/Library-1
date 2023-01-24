#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_3_A"
#include <iostream>
#include "src/Graph/BiConnectedComponents.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int V, E;
 cin >> V >> E;
 BiConnectedComponents bcc(V);
 for (int i= 0; i < E; ++i) {
  int s, t;
  cin >> s >> t;
  bcc.add_edge(s, t);
 }
 auto bct= bcc.block_cut_tree();
 for (int v= 0; v < V; ++v)
  if (bct[v].size() > 1) cout << v << '\n';
 return 0;
}