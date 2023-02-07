#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/5/GRL/5/GRL_5_A"
#include <iostream>
#include <algorithm>
#include "src/Graph/rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int n;
 cin >> n;
 Tree<int> tree(n);
 for (int i= 0; i < n - 1; ++i) {
  int s, t, w;
  cin >> s >> t >> w;
  tree.add_edge(s, t, w);
 }
 tree.build();
 auto f_ee= [&](int l, int r) { return max(l, r); };
 auto f_ve= [&](int d, int, const auto &e) { return d + e.cost; };
 auto f_ev= [&](int d, int) { return d; };
 auto dp= rerooting<int>(tree, f_ee, f_ve, f_ev, 0);
 cout << *max_element(dp.begin(), dp.end()) << '\n';
 return 0;
}