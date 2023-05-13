#define PROBLEM "https://judge.yosupo.jp/problem/tree_path_composite_sum"
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Graph/Tree.hpp"
#include "src/Graph/rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N;
 cin >> N;
 vector<Mint> a(N);
 for (int i= 0; i < N; ++i) cin >> a[i];
 using Cost= pair<Mint, Mint>;
 Tree<Cost> tree(N);
 for (int i= 0; i < N - 1; ++i) {
  int u, v, b, c;
  cin >> u >> v >> b >> c;
  tree.add_edge(u, v, Cost{b, c});
 }
 tree.build();
 using Data= pair<Mint, int>;
 auto f_ee= [&](const Data &l, const Data &r) { return Data{l.first + r.first, l.second + r.second}; };
 auto f_ev= [&](const Data &d, int v) { return Data{d.first + a[v], d.second + 1}; };
 auto f_ve= [&](Data d, int, const auto &e) {
  d.first= e.cost.first * d.first + e.cost.second * d.second;
  return d;
 };
 auto dp= rerooting<Data>(tree, f_ee, f_ve, f_ev, Data{0, 0});
 for (int i= 0; i < N; ++i) cout << dp[i].first << " \n"[i == N - 1];
 return 0;
}
