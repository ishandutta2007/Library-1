// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/nikkeiqual_2019/tasks/nikkei2019_2_qual_d
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 256
// https://atcoder.jp/contests/nikkei2019-2-qual/tasks/nikkei2019_2_qual_d
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include "src/Graph/RangeToRangeGraph.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 RangeToRangeGraph<int> r2r(N);
 for (int i= 0; i < M; ++i) {
  int L, R, C;
  cin >> L >> R >> C;
  --L;
  r2r.add_from_range_to_range(L, R, L, R, C);
 }
 int n= r2r.graph.vertex_size();
 auto adj= r2r.graph.adjacency_edge(1);
 long long dist[n];
 static constexpr long long INF= 1ll << 60;
 fill_n(dist, n, INF);
 priority_queue<pair<long long, int>> pq;
 pq.emplace(0, 0);
 dist[0]= 0;
 while (!pq.empty()) {
  auto [d, v]= pq.top();
  pq.pop();
  if (-d != dist[v]) continue;
  for (auto e: adj[v]) {
   int u= r2r.graph[e].to(v);
   int w= r2r.weight[e];
   long long cost= dist[v] + w;
   if (dist[u] > cost) {
    dist[u]= cost;
    pq.emplace(-cost, u);
   }
  }
 }
 cout << (dist[N - 1] == INF ? -1 : dist[N - 1]) << '\n';
 return 0;
}
