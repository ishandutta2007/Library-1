#define PROBLEM "https://atcoder.jp/contests/abc160/tasks/abc160_f"
#include <iostream>
#include <algorithm>
#include "src/Math/ModInt.hpp"
#include "src/Math/FactorialPrecalculation.hpp"
#include "src/Graph/rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<int(1e9 + 7)>;
 using F= FactorialPrecalculation<Mint>;
 int N;
 cin >> N;
 Tree tree(N);
 for (int i= 0; i < N - 1; ++i) {
  int a, b;
  cin >> a >> b;
  tree.add_edge(--a, --b);
 }
 tree.build();
 using Data= pair<int, Mint>;
 auto f_ee= [&](const Data &l, const Data &r) { return Data{l.first + r.first, F::nCr(l.first + r.first, l.first) * l.second * r.second}; };
 auto f_ve= [&](const Data &d, int, auto) { return d; };
 auto f_ev= [&](const Data &d, int) { return Data{d.first + 1, d.second}; };
 auto dp= rerooting<Data>(tree, f_ee, f_ve, f_ev, Data{0, 1});
 for (auto [_, x]: dp) cout << x << '\n';
 return 0;
}