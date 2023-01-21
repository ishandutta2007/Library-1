#define PROBLEM "https://judge.yosupo.jp/problem/general_weighted_matching"
#include <iostream>
#include <vector>
#include "src/Optimization/MatchingWeighted.hpp"
using namespace std;
int main() {
 int n, m;
 cin >> n >> m;
 auto solver= MatchingWeighted(n);
 for (int i= 0; i < m; i++) {
  int u, v, w;
  cin >> u >> v >> w;
  solver.add_edge(u, v, w);
 }
 auto [r1, r2, res]= solver.get_matching();
 cout << r1 << " " << r2 << '\n';
 for (int i= 0; i < n; i++)
  if (res[i] > i) cout << i << " " << res[i] << '\n';
 return 0;
}