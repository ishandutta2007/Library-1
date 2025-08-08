// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc220/tasks/abc220_f
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <array>
#include "src/Graph/Graph.hpp"
#include "src/Graph/Rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Graph g(N, N - 1);
 for (int i= 0; i < N - 1; ++i) cin >> g[i], --g[i];
 using Data= array<long long, 2>;
 auto put_edge= [&](int, int, const Data& d) { return Data{d[0], d[0] + d[1]}; };
 auto op= [&](const Data& l, const Data& r) { return Data{l[0] + r[0], l[1] + r[1]}; };
 auto put_vertex= [&](int, const Data& d) { return Data{d[0] + 1, d[1]}; };
 for (auto [_, x]: Rerooting<Data>(g, put_edge, op, Data{0, 0}, put_vertex)) cout << x << '\n';
 return 0;
}