#define PROBLEM "https://yukicoder.me/problems/no/2507"
#include <iostream>
#include "src/Graph/UndirectedGraphSetPowerSeries.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using GSPS= UndirectedGraphSetPowerSeries<15>;
 int n, m;
 cin >> n >> m;
 GSPS g(n);
 for (int i= 0; i < m; ++i) {
  int u, v;
  cin >> u >> v, --u, --v;
  g.add_edge(u, v);
 }
 auto f= g.cycle_graph<long long>();
 for (int i= n; i--;) f[1 << i]= 1;
 cout << g.disjoint_union(g.bridge_union(f)).back() << '\n';
 return 0;
}