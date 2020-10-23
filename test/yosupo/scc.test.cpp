#define PROBLEM "https://judge.yosupo.jp/problem/scc"
#include <bits/stdc++.h>
#include "src/Graph/StronglyConnectedComponents.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, M;
  cin >> N >> M;
  StronglyConnectedComponents graph(N);
  for (int i = 0; i < M; i++) {
    int a, b;
    cin >> a >> b;
    graph.add_edge(a, b);
  }
  auto ans = graph.get_SCC().first;
  cout << ans.size() << endl;
  for (auto &a : ans) {
    cout << a.size();
    for (int &v : a) cout << " " << v;
    cout << endl;
  }
  return 0;
}