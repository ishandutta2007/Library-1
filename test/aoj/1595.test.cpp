#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/challenges/search/titles/1595"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Graph/ReRooting.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N;
  cin >> N;
  auto op = [](int l, int r) { return max(l, r); };
  auto lift = [](int l, int dat) { return l + dat; };
  ReRooting<int> tree(N, op, 0, lift);
  for (int i = 0; i < N - 1; i++) {
    int u, v;
    cin >> u >> v;
    u--, v--;
    tree.add_edge(u, v, 1);
    tree.add_edge(v, u, 1);
  }
  auto ret = tree.run();
  for (int i = 0; i < N; i++) cout << (N - 1) * 2 - ret[i] << '\n';
  return 0;
}