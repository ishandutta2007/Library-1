#define PROBLEM "https://atcoder.jp/contests/abc213/tasks/abc213_g"
// 連結グラフ
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/Math/set_power_series.hpp"
#include "src/Graph/UndirectedGraphSetPowerSeries.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 int N, M;
 cin >> N >> M;
 int all= (1 << N) - 1;
 UndirectedGraphSetPowerSeries g(N);
 for (int i= 0, a, b; i < M; i++) cin >> a >> b, g.add_edge(--a, --b);
 auto x= g.graph<Mint>();
 auto y= g.connected_graph<Mint>();
 for (int k= 1; k < N; ++k) {
  Mint ans= 0;
  for (int s= 1; s <= all; s+= 2)
   if ((s >> k) & 1) ans+= y[s] * x[all ^ s];
  cout << ans << '\n';
 }
 return 0;
}
