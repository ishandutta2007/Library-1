#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2313"
#include <bits/stdc++.h>
#include "src/Graph/MaxFlow_Dinic.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, E, Q;
  cin >> N >> E >> Q;
  MaxFlow_Dinic<int> graph;
  graph.add_vertices(N);
  set<pair<int, int>> S;
  while (E--) {
    int F, T;
    cin >> F >> T;
    F--, T--;
    graph.add_edge(F, T, 1);
    graph.add_edge(T, F, 1);
    S.insert({T, F});
    S.insert({F, T});
  }
  int M[Q], A[Q], B[Q];
  int ans = graph.max_flow(0, N - 1);
  for (int q = 0; q < Q; q++) {
    cin >> M[q] >> A[q] >> B[q];
    A[q]--, B[q]--;
    if (S.count({A[q], B[q]})) continue;
    graph.add_edge(A[q], B[q], 0);
    graph.add_edge(B[q], A[q], 0);
  }
  for (int q = 0; q < Q; q++) {
    if (M[q] == 1) {
      ans += graph.link(A[q], B[q], 0, N - 1);
      ans += graph.link(B[q], A[q], 0, N - 1);
    } else {
      ans += graph.cut(A[q], B[q], 0, N - 1);
      ans += graph.cut(B[q], A[q], 0, N - 1);
    }
    cout << ans << endl;
  }
  return 0;
}