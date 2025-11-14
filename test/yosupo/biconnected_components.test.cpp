// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/biconnected_components
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Graph/Graph.hpp"
#include "src/Graph/block_cut_tree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 Graph g(N, M);
 for (int i= 0; i < M; ++i) cin >> g[i];
 auto bct= block_cut_tree(g).adjacency_vertex(0);
 int K= bct.size();
 cout << K - N << '\n';
 for (int i= N; i < K; ++i) {
  cout << bct[i].size();
  for (int v: bct[i]) cout << " " << v;
  cout << '\n';
 }
 return 0;
}