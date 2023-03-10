#define PROBLEM "https://atcoder.jp/contests/abc223/tasks/abc223_g"
#include <iostream>
#include <algorithm>
#include <array>
#include "src/Graph/rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Tree tree(N);
 for (int i= 0; i < N - 1; ++i) {
  int u, v;
  cin >> u >> v;
  tree.add_edge(--u, --v);
 }
 tree.build();
 using Data= array<int, 2>;
 auto f_ee= [&](const Data& l, const Data& r) { return Data{l[0] + r[0], max(l[1], r[1])}; };
 auto f_ve= [&](const Data& d, int, auto) {
  auto x= max(d[0], d[1]);
  return Data{x, d[0] + 1 - x};
 };
 auto f_ev= [&](const Data& d, int) { return Data{d[0], d[0] + d[1]}; };
 auto dp= rerooting<Data>(tree, f_ee, f_ve, f_ev, Data{0, 0});
 int mx= 0, ans= 0;
 for (auto [x, y]: dp) mx= max({mx, x, y});
 for (auto [x, y]: dp) ans+= (mx == x);
 cout << ans << '\n';
 return 0;
}