#define PROBLEM "https://judge.yosupo.jp/problem/general_weighted_matching"
#include <iostream>
#include <vector>
#include "src/Optimization/WeightedMatching.hpp"
using namespace std;
int main() {
 int n, m;
 cin >> n >> m;
 WeightedMatching<long long> graph(n);
 for (int i= 0; i < m; i++) {
  int u, v, w;
  cin >> u >> v >> w;
  graph.add_edge(u, v, w);
 }
 graph.build();
 auto ans= graph.weight_matching();
 long long sum= 0;
 for (auto [u, v, w]: ans) sum+= w;
 cout << ans.size() << " " << sum << '\n';
 for (int i= 0; i < n; i++)
  if (int j= graph.match(i); i < j) cout << i << " " << j << '\n';
 return 0;
}