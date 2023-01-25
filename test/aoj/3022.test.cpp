#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/3022"
#include <iostream>
#include <vector>
#include "src/Graph/BiConnectedComponents.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 vector<long long> w(N);
 for (int i= 0; i < N; ++i) cin >> w[i];
 BiConnectedComponents bcc(N);
 for (int i= 0; i < M; ++i) {
  int u, v;
  cin >> u >> v;
  bcc.add_edge(--u, --v);
 }
 auto bct= bcc.block_cut_tree();
 int K= bct.size();
 w.resize(K);
 vector<int> par(K);
 auto dfs= [&](auto self, int v, int p) -> void {
  par[v]= p;
  for (int u: bct[v])
   if (u != p) {
    self(self, u, v);
    w[v]+= w[u];
   }
 };
 dfs(dfs, 0, -1);
 for (int i= 0; i < N; ++i) {
  long long ans= w[0] - w[i];
  for (int u: bct[i])
   if (u != par[i]) ans= max(ans, w[u]);
  cout << ans << '\n';
 }
 return 0;
}