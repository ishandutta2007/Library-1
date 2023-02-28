#define PROBLEM "https://yukicoder.me/problems/no/1333"
#include <iostream>
#include <utility>
#include "src/Math/ModInt.hpp"
#include "src/Graph/rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<int(1e9 + 7)>;
 int N;
 cin >> N;
 Tree<Mint> tree(N);
 for (int i= 0; i < N - 1; ++i) {
  int u, v;
  Mint w;
  cin >> u >> v >> w;
  tree.add_edge(--u, --v, w);
 }
 tree.build();
 using Data= tuple<int, Mint, Mint>;
 auto f_ee= [&](const Data& l, const Data& r) { return Data{get<0>(l) + get<0>(r), get<1>(l) + get<1>(r), get<2>(l) + get<2>(r)}; };
 auto f_ve= [&](const Data& d, int, const auto& e) {
  auto [d0, d1, d2]= d;
  ++d0;
  d2+= (d1 * 2 + e.cost * d0) * e.cost;
  d1+= e.cost * d0;
  return Data{d0, d1, d2};
 };
 auto f_ev= [&](const Data& d, int) { return d; };
 auto dp= rerooting<Data>(tree, f_ee, f_ve, f_ev, Data{0, Mint(), Mint()});
 Mint ans= 0;
 for (auto [_, __, x]: dp) ans+= x;
 cout << ans / 2 << '\n';
 return 0;
}