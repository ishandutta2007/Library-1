#define PROBLEM "https://atcoder.jp/contests/abc223/tasks/abc223_g"
#include <iostream>
#include <algorithm>
#include <array>
#include "src/Graph/Graph.hpp"
#include "src/Graph/Rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Graph g(N - 1);
 for (int i= 0; i < N - 1; ++i) cin >> g[i], --g[i];
 g.build(N, 0);
 using Data= array<int, 2>;
 auto put_edge= [&](int, int, const Data& d) {
  auto x= max(d[0], d[1]);
  return Data{x, d[0] + 1 - x};
 };
 auto op= [&](const Data& l, const Data& r) { return Data{l[0] + r[0], max(l[1], r[1])}; };
 auto put_vertex= [&](int, const Data& d) { return Data{d[0], d[0] + d[1]}; };
 Rerooting<Data> dp(g, put_edge, op, Data{0, 0}, put_vertex);
 int mx= 0, ans= 0;
 for (auto [x, y]: dp) mx= max({mx, x, y});
 for (auto [x, y]: dp) ans+= (mx == x);
 cout << ans << '\n';
 return 0;
}