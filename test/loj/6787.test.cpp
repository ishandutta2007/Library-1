// competitive-verifier: PROBLEM https://loj.ac/p/6787
// competitive-verifier: TLE 1.0
// competitive-verifier: MLE 128

#include <iostream>
#include "mylib/algebra/ModInt.hpp"
#include "mylib/graph/UndirectedGraphSetPowerSeries.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 int n;
 cin >> n;
 UndirectedGraphSetPowerSeries g(n);
 for(int i= 0; i < n; ++i)
  for(int j= 0; j < n; ++j) {
   bool c;
   cin >> c;
   if(c && i < j) g.add_edge(i, j);
  }
 auto f= g.chromatic_polynomial<Mint>();
 for(int i= 0; i <= n; ++i) cout << f[i] << " \n"[i == n];
 return 0;
}
