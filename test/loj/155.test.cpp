// competitive-verifier: PROBLEM https://loj.ac/p/155
// competitive-verifier: TLE 1.5
// competitive-verifier: MLE 256

#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Math/set_power_series.hpp"
#include "src/Graph/UndirectedGraphSetPowerSeries.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 int n;
 cin >> n;
 UndirectedGraphSetPowerSeries g(n);
 for (int i= 0; i < n; ++i)
  for (int j= 0; j < n; ++j) {
   bool c;
   cin >> c;
   if (c && i < j) g.add_edge(i, j);
  }
 Mint x, y;
 cin >> x >> y;
 cout << g.tutte_polynomial(x, y) << '\n';
 return 0;
}