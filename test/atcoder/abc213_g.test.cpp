#define PROBLEM "https://atcoder.jp/contests/abc213/tasks/abc213_g"
// 連結グラフ
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/Math/SetPowerSeries.hpp"
#include "src/Graph/UndirectedGraphSetPowerSeries.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 int N, M;
 cin >> N >> M;
 UndirectedGraphSetPowerSeries<17> g(N);
 for (int i= 0; i < M; i++) {
  int a, b;
  cin >> a >> b;
  g.add_edge(--a, --b);
 }
 auto tmp= g.graph<Mint>();
 auto tmp2= g.connected_graph<Mint>();
 for (int k= 1; k < N; k++) {
  Mint ans= 0;
  for (int s= 1; s < (1 << N); s+= 2)
   if ((s >> k) & 1) ans+= tmp2[s] * tmp[((1 << N) - 1) ^ s];
  cout << ans << '\n';
 }
 return 0;
}
