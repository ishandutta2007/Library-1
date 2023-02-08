#define PROBLEM "https://atcoder.jp/contests/abc220/tasks/abc220_f"
#include <iostream>
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
 using Data= array<long long, 2>;
 auto f_ee= [&](const Data& l, const Data& r) { return Data{l[0] + r[0], l[1] + r[1]}; };
 auto f_ve= [&](const Data& d, int, auto) { return Data{d[0], d[0] + d[1]}; };
 auto f_ev= [&](const Data& d, int) { return Data{d[0] + 1, d[1]}; };
 auto dp= rerooting<Data>(tree, f_ee, f_ve, f_ev, Data{0, 0});
 for (auto [_, x]: dp) cout << x << '\n';
 return 0;
}