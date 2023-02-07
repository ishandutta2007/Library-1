#define PROBLEM "https://yukicoder.me/problems/no/1718"
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include "src/Graph/rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, K;
 cin >> N >> K;
 Tree tree(N);
 for (int i= 0; i < N - 1; ++i) {
  int u, v;
  cin >> u >> v;
  tree.add_edge(--u, --v);
 }
 tree.build();
 vector<int> cnt(N);
 for (int i= 0; i < K; ++i) {
  int D;
  cin >> D;
  ++cnt[--D];
 }
 using Data= array<int, 2>;
 auto f_ee= [&](const Data& l, const Data& r) { return Data{l[0] + r[0], max(l[1], r[1])}; };
 auto f_ve= [&](Data d, int, auto e) {
  if (cnt[e.to] || d[0]) d[0]+= 2, d[1]+= 1;
  return d;
 };
 auto f_ev= [&](const Data& d, int v) { return d; };
 auto dp= rerooting<Data>(tree, f_ee, f_ve, f_ev, Data{0, 0});
 for (auto [a, b]: dp) cout << a - b << '\n';
 return 0;
}