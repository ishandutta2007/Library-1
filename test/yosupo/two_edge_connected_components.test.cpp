#define PROBLEM "https://judge.yosupo.jp/problem/two_edge_connected_components"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Graph/TwoEdgeConnectedComponents.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, M;
  cin >> N >> M;
  TwoEdgeConnectedComponents graph(N);
  for (int i = 0; i < M; i++) {
    int a, b;
    cin >> a >> b;
    graph.add_edge(a, b);
  }
  auto ans = graph.get_2ECC().first;
  cout << ans.size() << endl;
  for (auto &a : ans) {
    cout << a.size();
    for (int &v : a) cout << " " << v;
    cout << endl;
  }
  return 0;
}