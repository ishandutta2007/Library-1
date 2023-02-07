#define PROBLEM "https://atcoder.jp/contests/arc097/tasks/arc097_d"
#include <iostream>
#include <string>
#include <array>
#include <tuple>
#include <algorithm>
#include "src/Graph/rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Tree tree(N);
 for (int i= 0; i < N - 1; ++i) {
  int x, y;
  cin >> x >> y;
  tree.add_edge(--x, --y);
 }
 tree.build();
 string c;
 cin >> c;
 using Dat= array<int, 2>;
 using Data= tuple<bool, int, int>;
 auto f_ee= [&](const Data &l, const Data &r) { return Data{get<0>(l) ^ get<0>(r), get<1>(l) + get<1>(r), max(get<2>(l), get<2>(r))}; };
 auto f_ve= [&](const Dat &d, int, auto) { return d[1] ? Data{1, 2 + d[0], d[0] - d[1] + 1} : Data{0, 0, 0}; };
 auto f_ev= [&](const Data &d, int v) {
  auto [b, x, y]= d;
  b^= (c[v] == 'B');
  return Dat{x + b, x - y + !b};
 };
 auto dp= rerooting<Dat>(tree, f_ee, f_ve, f_ev, Data{0, 0, 0});
 int ans= 1 << 30;
 for (auto [_, x]: dp) ans= min(ans, x);
 cout << ans << '\n';
 return 0;
}