// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/3022
#include <iostream>
#include <vector>
#include "src/Graph/Graph.hpp"
#include "src/Graph/block_cut_tree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 vector<long long> w(N);
 for (int i= 0; i < N; ++i) cin >> w[i];
 Graph g(N, M);
 for (int i= 0; i < M; ++i) cin >> g[i], --g[i];
 auto bct= block_cut_tree(g).adjacency_vertex(0);
 int K= bct.size();
 w.resize(K);
 vector<int> par(K);
 auto dfs= [&](auto&& dfs, int v, int p) -> void {
  par[v]= p;
  for (int u: bct[v])
   if (u != p) dfs(dfs, u, v), w[v]+= w[u];
 };
 dfs(dfs, 0, -1);
 for (int v= 0; v < N; ++v) {
  long long ans= w[0] - w[v];
  for (int u: bct[v])
   if (u != par[v]) ans= max(ans, w[u]);
  cout << ans << '\n';
 }
 return 0;
}