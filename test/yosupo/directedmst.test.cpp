#define PROBLEM "https://judge.yosupo.jp/problem/directedmst"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "DataStructure/UnionFind.hpp"
#include "DataStructure/SkewHeap.hpp"
#include "Graph/MinimumSpanningAborescense.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, M, r;
  cin >> N >> M >> r;
  MinimumSpanningAborescense<long long> graph(N);
  for (int i = 0; i < M; i++) {
    int s, t, w;
    cin >> s >> t >> w;
    graph.add_edge(s, t, w);
  }
  auto ans = graph.get_MSA(r);
  vector<int> par(N);
  iota(par.begin(), par.end(), 0);
  for (auto e : ans.second) par[e.dst] = e.src;
  cout << ans.first << endl;
  for (int i = 0; i < N; i++) cout << (i ? " " : "") << par[i];
  cout << endl;
  return 0;
}