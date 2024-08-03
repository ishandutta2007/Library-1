// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc204/tasks/abc204_e
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include "src/Optimization/fibonacci_search.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 int A[M], B[M];
 long long C[M], D[M];
 vector<int> adj[N];
 for (int i= 0; i < M; ++i) {
  cin >> A[i] >> B[i] >> C[i] >> D[i], --A[i], --B[i];
  adj[A[i]].push_back(i), adj[B[i]].push_back(i);
 }
 priority_queue<pair<long long, int>> pq;
 static constexpr long long INF= 1e18;
 long long dist[N];
 fill_n(dist, N, INF);
 dist[0]= 0, pq.emplace(0, 0);
 while (!pq.empty()) {
  auto [d, u]= pq.top();
  pq.pop();
  d= -d;
  if (dist[u] != d) continue;
  for (auto e: adj[u]) {
   int v= A[e] ^ B[e] ^ u;
   auto f= [&](long long t) { return t + C[e] + double(D[e]) / (t + 1); };
   auto [_, nd_f]= fibonacci_search<MINIMIZE>(f, d, max(d, D[e]));
   long long nd= floor(nd_f) + 0.5;
   if (dist[v] > nd) dist[v]= nd, pq.emplace(-nd, v);
  }
 }
 cout << (dist[N - 1] == INF ? -1 : dist[N - 1]) << '\n';
 return 0;
}