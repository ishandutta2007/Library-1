#define PROBLEM \
  "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_2_B"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/DataStructure/UnionFind.hpp"
#include "src/DataStructure/SkewHeap.hpp"
#include "src/Graph/MinimumSpanningAborescense.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, M, r;
  cin >> N >> M >> r;
  MinimumSpanningAborescense<int> graph(N);
  for (int i = 0; i < M; i++) {
    int s, t, w;
    cin >> s >> t >> w;
    graph.add_edge(s, t, w);
  }
  cout << graph.get_MSA(r).first << endl;
  return 0;
}