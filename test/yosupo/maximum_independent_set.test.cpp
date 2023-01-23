#define PROBLEM "https://old.yosupo.jp/problem/maximum_independent_set"
#include <iostream>
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
 auto ans= graph.get_max_independent_set();
 int n= ans.size();
 cout << n << '\n';
 for (int i= 0; i < n; i++) cout << ans[i] << " \n"[i == n - 1];
 return 0;
}