#define PROBLEM "https://judge.yosupo.jp/problem/bipartitematching"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "Graph/MatchingBipartite.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int L, R, M;
  cin >> L >> R >> M;
  MatchingBipartite graph(L, R);
  while (M--) {
    int a, b;
    cin >> a >> b;
    graph.add_edge(a, b);
  }
  auto ans = graph.get_matching();
  auto left = ans.second.first;
  cout << ans.first << endl;
  for (int i = 0; i < left.size(); i++)
    if (left[i] != -1) {
      cout << i << " " << left[i] << endl;
    }
  return 0;
}