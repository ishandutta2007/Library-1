#define PROBLEM "https://judge.yosupo.jp/problem/bipartite_edge_coloring"
#include <iostream>
#include <algorithm>
#include "src/Graph/BipartiteEdgeColoring.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int L, R, M;
 cin >> L >> R >> M;
 BipartiteEdgeColoring graph(L, R);
 for (int i= 0; i < M; ++i) {
  int a, b;
  cin >> a >> b;
  graph.add_edge(a, b);
 }
 auto ans= graph.edge_coloring();
 cout << *max_element(ans.begin(), ans.end()) + 1 << '\n';
 for (auto a: ans) cout << a << '\n';
 return 0;
}