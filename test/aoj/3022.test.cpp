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
 bct.build(0);
 int K= bct.size();
 w.resize(K);
 for (int v= K; v--;)
  if (int u= bct.parent(v); u != -1) w[u]+= w[v];
 for (int i= 0; i < N; ++i) {
  long long ans= w[0] - w[i];
  for (int u: bct[i])
   if (u != bct.parent(i)) ans= max(ans, w[u]);
  cout << ans << '\n';
 }
 return 0;
}