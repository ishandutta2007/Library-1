// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1124
// competitive-verifier: TLE 0.5
#include <iostream>
#include <array>
#include "src/Math/ModInt.hpp"
#include "src/Graph/Graph.hpp"
#include "src/Graph/Rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<int(1e9 + 7)>;
 int N;
 cin >> N;
 Graph g(N, N - 1);
 for (int i= 0; i < N - 1; ++i) cin >> g[i], --g[i];
 using Data= array<Mint, 2>;
 static constexpr Mint iv2= Mint(1) / 2;
 auto put_edge= [&](int, int, const Data &d) { return Data{d[0] * iv2, d[1] * iv2}; };
 auto op= [&](const Data &l, const Data &r) { return Data{l[0] + r[0], l[1] + l[0] * r[0] * 2 + r[1]}; };
 auto put_vertex= [&](int, const Data &d) { return Data{d[0] + 1, d[1] + d[0] * 2 + 1}; };
 Mint ans= 0;
 for (auto [_, x]: Rerooting<Data>(g, put_edge, op, Data{0, 0}, put_vertex)) ans+= x;
 ans*= Mint(2).pow(N - 1);
 cout << ans << '\n';
 return 0;
}