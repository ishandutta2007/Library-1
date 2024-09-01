// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/6/GRL_6_A
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Optimization/MaxFlow.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 MaxFlow<Dinic<long long>> graph(N);
 for (int i= 0; i < M; i++) {
  int u, v;
  long long c;
  cin >> u >> v >> c;
  graph.add_edge(u, v, c);
 }
 cout << graph.maxflow(0, N - 1) << endl;
 return 0;
}