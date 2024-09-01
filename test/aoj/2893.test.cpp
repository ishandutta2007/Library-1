// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/2893
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <algorithm>
#include "src/Graph/Graph.hpp"
#include "src/Misc/compress.hpp"
#include "src/Graph/IncrementalBridgeConnectivity.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 Graph g(N, M);
 vector<long long> w(M);
 for (int i= 0; i < M; ++i) cin >> g[i] >> w[i], --g[i];
 IncrementalBridgeConnectivity ibc(N);
 for (auto [u, v]: g) ibc.add_edge(u, v);

 vector<int> id(N);
 int n= 0;
 for (int i= 0; i < N; ++i)
  if (ibc.leader(i) == i) id[i]= n++;

 Graph g2(n);
 vector<long long> s(n, 0), w2;
 for (int e= M; e--;) {
  auto [u, v]= g[e];
  u= id[ibc.leader(u)], v= id[ibc.leader(v)];
  if (u == v) s[u]+= w[e];
  else g2.add_edge(u, v), w2.push_back(w[e]);
 }

 auto adje= g2.adjacency_edge(0);

 auto dfs= [&](auto &&dfs, int v, int p) -> void {
  for (int e: adje[v])
   if (int u= g2[e].to(v); u != p) {
    dfs(dfs, u, v);
    s[v]+= s[u] + w2[e];
   }
 };
 dfs(dfs, 0, -1);

 int ans_u= 0, ans_v= 0;
 long long best= 1ll << 60;

 for (int e= 0; e < M; ++e) {
  auto [u, v]= g[e];
  int u2= id[ibc.leader(u)], v2= id[ibc.leader(v)];
  long long cost;
  if (u2 == v2) cost= s[0] - w[e];
  else {
   if (s[u2] > s[v2]) swap(u2, v2);
   long long WA= s[u2], WB= s[0] - s[u2] - w[e];
   cost= abs(WA - WB);
  }
  if (best > cost) best= cost, ans_u= u, ans_v= v;
  else if (best == cost) {
   if (ans_u > u) ans_u= u, ans_v= v;
   else if (ans_u == u && ans_v > v) ans_v= v;
  }
 }

 cout << ans_u + 1 << " " << ans_v + 1 << '\n';
 return 0;
}