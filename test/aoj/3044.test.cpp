// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/3044
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include "src/Optimization/fibonacci_search.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M, S, G;
 cin >> N >> M >> S >> G;
 --S, --G;
 int U[M], V[M];
 long long A[M], B[M];
 vector<int> adj[N];
 for (int i= 0; i < M; ++i) {
  cin >> U[i] >> V[i] >> A[i] >> B[i], --U[i], --V[i];
  adj[U[i]].push_back(i), adj[V[i]].push_back(i);
 }
 priority_queue<pair<long long, int>> pq;
 static constexpr long long INF= 1e18;
 long long dist[N];
 fill_n(dist, N, INF);
 dist[S]= 0, pq.emplace(0, S);
 while (!pq.empty()) {
  auto [d, u]= pq.top();
  pq.pop();
  d= -d;
  if (dist[u] != d) continue;
  for (auto e: adj[u]) {
   int v= U[e] ^ V[e] ^ u;
   auto f= [&](long long x) { return x + double(B[e]) / (x + A[e]); };
   auto [_, nd_f]= fibonacci_search<MINIMIZE>(f, d, max(d, B[e]));
   long long nd= ceil(nd_f) + 0.5;
   if (dist[v] > nd) dist[v]= nd, pq.emplace(-nd, v);
  }
 }
 cout << (dist[G] == INF ? -1 : dist[G]) << '\n';
 return 0;
}