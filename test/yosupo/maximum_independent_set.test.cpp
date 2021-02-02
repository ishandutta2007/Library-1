#define PROBLEM "https://old.yosupo.jp/problem/maximum_independent_set"
#include <bits/stdc++.h>
#include "src/Graph/MaxClique.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, M;
  cin >> N >> M;
  MaxClique graph(N);
  while (M--) {
    int u, v;
    cin >> u >> v;
    graph.add_edge(u, v);
  }
  auto ans = graph.get_max_independent_set();
  cout << ans.size() << '\n';
  for (int i = 0; i < (int)ans.size(); i++) cout << (i ? " " : "") << ans[i];
  cout << '\n';
  return 0;
}