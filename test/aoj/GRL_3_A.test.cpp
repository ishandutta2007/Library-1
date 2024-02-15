#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/all/GRL_3_A"
#include <iostream>
#include "src/Graph/Graph.hpp"
#include "src/Graph/block_cut_tree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int V, E;
 cin >> V >> E;
 Graph g(E);
 for (int i= 0; i < E; ++i) cin >> g[i];
 g.build(V, 0);
 auto bct= block_cut_tree(g);
 for (int v= 0; v < V; ++v)
  if (bct(v).size() > 1) cout << v << '\n';
 return 0;
}