#define PROBLEM "https://atcoder.jp/contests/abc160/tasks/abc160_f"
#include <iostream>
#include <algorithm>
#include "src/Math/ModInt.hpp"
#include "src/Math/FactorialPrecalculation.hpp"
#include "src/Graph/Graph.hpp"
#include "src/Graph/Rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<int(1e9 + 7)>;
 using F= FactorialPrecalculation<Mint>;
 int N;
 cin >> N;
 Graph g(N, N - 1);
 for (int i= 0; i < N - 1; ++i) cin >> g[i], --g[i];
 using Data= pair<int, Mint>;
 auto put_edge= [&](int, int, const Data &d) { return d; };
 auto op= [&](const Data &l, const Data &r) { return Data{l.first + r.first, F::nCr(l.first + r.first, l.first) * l.second * r.second}; };
 auto put_vertex= [&](int, const Data &d) { return Data{d.first + 1, d.second}; };
 for (auto [_, x]: Rerooting<Data>(g, put_edge, op, Data{0, 1}, put_vertex)) cout << x << '\n';
 return 0;
}