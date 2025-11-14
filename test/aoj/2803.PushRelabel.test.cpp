// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/2803
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <algorithm>
#include "src/Optimization/MaxFlow.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int K, N, M;
 cin >> K >> N >> M;
 const long long INF= 512345;
 using MF= MaxFlow<PushRelabel<long long>>;
 MF graph(N + K + 1);
 vector<MF::EdgePtr> edges;
 int src= graph.add_vertex();
 for (int j= 1; j <= K; j++) graph.add_edge(src, j, INF);
 for (int i= 0; i < M; i++) {
  int a, b, c;
  cin >> a >> b >> c;
  edges.emplace_back(graph.add_edge(a, b, c, true));
 }
 long long flow= graph.maxflow(src, 0);
 long long add= 0;
 auto S= graph.mincut(src);
 for (auto &e: edges)
  if (S[e.src()] != S[e.dst()]) {
   long long pre= e.cap();
   e.change_cap(INF, src, 0);
   add= max(add, graph.maxflow(src, 0));
   e.change_cap(pre, src, 0);
  }
 flow+= add;
 if (flow >= INF) cout << "overfuro" << '\n';
 else cout << flow << '\n';
 return 0;
}