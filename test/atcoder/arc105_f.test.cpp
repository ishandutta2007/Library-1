#define PROBLEM "https://atcoder.jp/contests/arc105/tasks/arc105_f"
// 連結二部グラフ
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
 cout << g.connected_biparate_graph<Mint>().back() << '\n';
 return 0;
}
