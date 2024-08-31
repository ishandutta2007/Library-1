// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/tree_path_composite_sum
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Graph/Graph.hpp"
#include "src/Graph/Rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N;
 cin >> N;
 vector<Mint> a(N);
 for (int i= 0; i < N; ++i) cin >> a[i];
 Graph g(N, N - 1);
 vector<Mint> b(N - 1), c(N - 1);
 for (int i= 0; i < N - 1; ++i) cin >> g[i] >> b[i] >> c[i];
 using Data= pair<Mint, int>;
 auto put_edge= [&](int, int e, const Data &d) { return Data{b[e] * d.first + c[e] * d.second, d.second}; };
 auto op= [&](const Data &l, const Data &r) { return Data{l.first + r.first, l.second + r.second}; };
 auto put_vertex= [&](int v, const Data &d) { return Data{d.first + a[v], d.second + 1}; };
 Rerooting<Data> dp(g, put_edge, op, Data{0, 0}, put_vertex);
 for (int i= 0; i < N; ++i) cout << dp[i].first << " \n"[i == N - 1];
 return 0;
}