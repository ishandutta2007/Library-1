// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/maximum_independent_set
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Graph/CliqueProblem.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 CliqueProblem graph(N);
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