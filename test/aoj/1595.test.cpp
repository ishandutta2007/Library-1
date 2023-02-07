#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/challenges/sources/UOA/UAPC/1595"
#include <iostream>
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
 auto f_ee= [&](int l, int r) { return max(l, r); };
 auto f_ve= [&](int d, int, auto) { return d + 1; };
 auto f_ev= [&](int d, int) { return d; };
 auto dp= rerooting<int>(tree, f_ee, f_ve, f_ev, 0);
 for (auto x: dp) cout << 2 * (N - 1) - x << '\n';
 return 0;
}