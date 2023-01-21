#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2893"
#include <iostream>
#include <vector>
#include <algorithm>
#include "src/Misc/compress.hpp"
#include "src/Graph/IncrementalBridgeConnectivity.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 IncrementalBridgeConnectivity ibc(N);
 vector<int> u(M), v(M);
 vector<long long> w(M);
 for (int i= 0; i < M; ++i) {
  cin >> u[i] >> v[i] >> w[i];
  ibc.add_edge(--u[i], --v[i]);
 }
 vector<int> vec;
 for (int i= 0; i < N; ++i) vec.push_back(ibc.represent(i));
 auto id= compress(vec);
 int n= vec.size();
 vector<vector<pair<int, int>>> adj(n);
 vector<long long> s(n, 0);
 for (int i= 0; i < M; ++i) {
  int j= ibc.represent(u[i]), k= ibc.represent(v[i]);
  j= id(j), k= id(k);
  if (j == k) s[j]+= w[i];
  else adj[j].emplace_back(k, i), adj[k].emplace_back(j, i);
 }

 int ans_u= 0, ans_v= 0;
 long long max_c= 1ll << 60;

 auto dfs= [&](auto self, int x, int p) -> void {
  for (auto [y, i]: adj[x])
   if (y != p) {
    self(self, y, x);
    s[x]+= s[y] + w[i];
   }
 };
 dfs(dfs, 0, -1);

 for (int e= 0; e < M; ++e) {
  int j= ibc.represent(u[e]), k= ibc.represent(v[e]);
  j= id(j), k= id(k);
  long long cost;
  if (j == k) {
   cost= s[0] - w[e];
  } else {
   if (s[j] > s[k]) swap(j, k);
   long long WA= s[j], WB= s[0] - s[j] - w[e];
   cost= abs(WA - WB);
  }
  if (max_c > cost) max_c= cost, ans_u= u[e], ans_v= v[e];
  else if (max_c == cost) {
   if (ans_u > u[e]) ans_u= u[e], ans_v= v[e];
   else if (ans_u == u[e] && ans_v > v[e]) ans_v= v[e];
  }
 }

 cout << ans_u + 1 << " " << ans_v + 1 << '\n';
 return 0;
}