// competitive-verifier: PROBLEM https://atcoder.jp/contests/arc099/tasks/arc097_F
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// https://atcoder.jp/contests/arc097/tasks/arc097_d
#include <iostream>
#include <string>
#include <array>
#include <tuple>
#include <algorithm>
#include "src/Graph/Rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Graph g(N, N - 1);
 for (int i= 0; i < N - 1; ++i) cin >> g[i], --g[i];
 string c;
 cin >> c;
 using T= array<int, 2>;
 using U= tuple<bool, int, int>;
 auto put_edge= [&](int, int, const T &d) { return d[1] ? U{1, 2 + d[0], d[0] - d[1] + 1} : U{0, 0, 0}; };
 auto op= [&](const U &l, const U &r) {
  auto [b1, x1, y1]= l;
  auto [b2, x2, y2]= r;
  return U{b1 ^ b2, x1 + x2, max(y1, y2)};
 };
 auto put_vertex= [&](int v, const U &d) {
  auto [b, x, y]= d;
  b^= (c[v] == 'B');
  return T{x + b, x - y + !b};
 };
 int ans= 1 << 30;
 for (auto [_, x]: Rerooting<T>(g, put_edge, op, U{0, 0, 0}, put_vertex)) ans= min(ans, x);
 cout << ans << '\n';
 return 0;
}