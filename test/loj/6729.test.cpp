// competitive-verifier: PROBLEM https://loj.ac/p/6729
// competitive-verifier: TLE 1.0
// competitive-verifier: MLE 64

#include <iostream>
#include "mylib/algebra/ModInt.hpp"
#include "mylib/graph/UndirectedGraphSetPowerSeries.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 int n, m;
 cin >> n >> m;
 UndirectedGraphSetPowerSeries g(n);
 for(int i= m; i--;) {
  int u, v;
  cin >> u >> v, --u, --v;
  g.add_edge(u, v);
 }
 if(n == 1) cout << 1 << '\n';
 else cout << g.biconnected_graph<Mint>().back() << '\n';
 return 0;
}
