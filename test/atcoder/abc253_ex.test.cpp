// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc253/tasks/abc253_Ex
// https://atcoder.jp/contests/abc253/tasks/abc253_h
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
 UndirectedGraphSetPowerSeries g(N);
 for (int i= 0, u, v; i < M; i++) cin >> u >> v, g.add_edge(--u, --v);
 auto tree= g.tree<Mint>();
 auto f= sps::egf_T(tree);
 Mint fact= 1, iv= Mint(1) / M, pw= 1;
 for (int i= 1; i < N; i++) cout << f[N - i] * (fact*= i) * (pw*= iv) << '\n';
 return 0;
}