// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1333
#include <iostream>
#include <tuple>
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
 vector<Mint> w(N - 1);
 for (int i= 0; i < N - 1; ++i) cin >> g[i] >> w[i], --g[i];
 using Data= tuple<int, Mint, Mint>;
 auto put_edge= [&](int, int e, const Data& d) {
  auto [d0, d1, d2]= d;
  return Data{d0 + 1, d1 + w[e] * d0, d2 + w[e] * (d1 + d1 + w[e] * d0)};
 };
 auto op= [&](const Data& l, const Data& r) {
  auto [l0, l1, l2]= l;
  auto [r0, r1, r2]= r;
  return Data{l0 + r0, l1 + r1, l2 + r2};
 };
 auto put_vertex= [&](int, const Data& d) { return d; };
 Mint ans= 0;
 for (auto [_, __, x]: Rerooting<Data>(g, put_edge, op, Data{0, 0, 0}, put_vertex)) ans+= x;
 cout << ans / 2 << '\n';
 return 0;
}