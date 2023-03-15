#define PROBLEM "https://judge.yosupo.jp/problem/general_matching"
#include <iostream>
#include "src/Graph/Matching.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 Matching graph(N);
 while (M--) {
  int u, v;
  cin >> u >> v;
  graph.add_edge(u, v);
 }
 graph.build();
 cout << graph.max_matching().size() << '\n';
 for (int i= 0; i < N; i++)
  if (int j= graph.match(i); i < j) cout << i << " " << j << '\n';
 return 0;
}