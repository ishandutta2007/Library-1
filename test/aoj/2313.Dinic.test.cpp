#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/challenges/sources/JAG/Summer/2313"
#include <bits/stdc++.h>
#include "src/Optimization/Dinic.hpp"
#include "src/Optimization/MaxFlow.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, E, Q;
  cin >> N >> E >> Q;
  using MF = MaxFlow<Dinic<long long>>;
  MF graph(N);
  vector<MF::EdgePtr> edges;
  int ei[N][N];
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++) ei[i][j] = -1;
  for (int i = 0; i < E; i++) {
    int F, T;
    cin >> F >> T, F--, T--;
    if (F > T) swap(F, T);
    ei[F][T] = edges.size();
    edges.emplace_back(graph.add_edge(F, T, 1, true));
  }
  int M[Q], A[Q], B[Q];
  for (int i = 0; i < Q; i++) {
    cin >> M[i] >> A[i] >> B[i], A[i]--, B[i]--;
    if (A[i] > B[i]) swap(A[i], B[i]);
    if (ei[A[i]][B[i]] == -1) {
      ei[A[i]][B[i]] = edges.size();
      edges.emplace_back(graph.add_edge(A[i], B[i], 0, true));
    }
  }
  long long flow = graph.maxflow(0, N - 1);
  for (int i = 0; i < Q; i++) {
    int e = ei[A[i]][B[i]];
    flow -= edges[e].change_cap(M[i] == 1, 0, N - 1);
    flow += graph.maxflow(0, N - 1, 1);
    cout << flow << '\n';
  }
  return 0;
}