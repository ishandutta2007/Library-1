#define PROBLEM "https://judge.yosupo.jp/problem/general_matching"
#include <bits/stdc++.h>
#include "src/Graph/MatchingGeneral.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, M;
  cin >> N >> M;
  MatchingGeneral graph(N);
  while (M--) {
    int u, v;
    cin >> u >> v;
    graph.add_edge(u, v);
  }
  auto ans = graph.get_matching();
  cout << ans.first << endl;
  for (int i = 0; i < N; i++)
    if (i < ans.second[i]) cout << i << " " << ans.second[i] << endl;
  return 0;
}