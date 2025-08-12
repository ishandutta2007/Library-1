// competitive-verifier: PROBLEM https://loj.ac/p/2340
// competitive-verifier: TLE 4
// competitive-verifier: MLE 512

#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Math/set_power_series.hpp"
#include "src/Graph/UndirectedGraphSetPowerSeries.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int n, m, p;
 cin >> n >> m >> p;
 int N= 1 << n;
 UndirectedGraphSetPowerSeries g(n);
 for (int i= 0; i < m; ++i) {
  int u, v;
  cin >> u >> v, --u, --v;
  g.add_edge(u, v);
 }
 vector<Mint> w(N);
 for (int i= 0; i < n; ++i) cin >> w[1 << i];
 sps::subset_zeta(w);
 for (int s= N; --s;) w[s]= w[s].pow(p);
 auto comp= g.connected_component_num();
 auto odd= g.odd_deg_num();
 vector<Mint> f(N);
 for (int s= N; --s;)
  if (comp[s] != 1 || odd[s] != 0) f[s]= w[s];
 auto phi= [&](int s, Mint &x) { x/= w[s]; };
 auto ans= sps::semi_relaxed_convolve<Mint>(f, 1, phi);
 cout << ans.back() << '\n';
 return 0;
}