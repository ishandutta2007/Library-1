#define PROBLEM "https://atcoder.jp/contests/abc223/tasks/abc223_g"
// DM分解のverify
#include <iostream>
#include <vector>
#include <algorithm>
#include "src/Graph/BipartiteGraph.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 BipartiteGraph graph(N);
 for (int i= 0; i < N - 1; ++i) {
  int u, v;
  cin >> u >> v;
  graph.add_edge(--u, --v);
 }
 graph.build();
 int k= graph.component_num();
 int ans= 0;
 for (int v: graph.block(0))
  if (graph.color(v)) ++ans;
 for (int v: graph.block(k - 1))
  if (!graph.color(v)) ++ans;
 cout << ans << '\n';
 return 0;
}