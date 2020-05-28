#define PROBLEM \
  "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_2_A"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "DataStructure/UnionFind.hpp"
#include "Graph/MinimumSpanningTree_Kruskal.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  int N, M;
  cin >> N >> M;
  MinimumSpanningTree_Kruskal<int> graph(N);
  for (int i = 0; i < M; i++) {
    int s, t, w;
    cin >> s >> t >> w;
    graph.add_edge(s, t, w);
  }
  cout << graph.get_MST().first << endl;
  return 0;
}