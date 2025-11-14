// competitive-verifier: PROBLEM https://loj.ac/p/6719
// competitive-verifier: TLE 1.0
// competitive-verifier: MLE 64

#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/Graph/UndirectedGraphSetPowerSeries.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 int n, m;
 cin >> n >> m;
 UndirectedGraphSetPowerSeries g(n);
 for (int i= m; i--;) {
  int u, v;
  cin >> u >> v, --u, --v;
  g.add_edge(u, v);
 }
 cout << g.cactus_graph<Mint>().back() << '\n';
 return 0;
}