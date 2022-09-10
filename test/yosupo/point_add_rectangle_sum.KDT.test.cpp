#define PROBLEM "https://judge.yosupo.jp/problem/point_add_rectangle_sum"
#include <bits/stdc++.h>
#include "src/DataStructure/KDTree.hpp"
using namespace std;

struct RSQ {
  using T = long long;
  static T ti() { return 0; }
  static T op(const T &l, const T &r) { return l + r; }
};
signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  int N, Q;
  cin >> N >> Q;
  map<array<long long, 2>, long long> mp;
  vector<array<long long, 5>> query;
  for (int i = 0; i < N; i++) {
    long long x, y, w;
    cin >> x >> y >> w;
    mp[{x, y}] += w;
  }
  for (int i = 0; i < Q; i++) {
    int op;
    cin >> op;
    if (op) {
      int l, d, r, u;
      cin >> l >> d >> r >> u;
      query.push_back({op, l, d, r, u});
    } else {
      int x, y, w;
      cin >> x >> y >> w;
      query.push_back({op, x, y, w});
      mp[{x, y}];
    }
  }
  KDTree<2, long long, RSQ> kdt({mp.begin(), mp.end()});
  for (int i = 0; i < Q; i++) {
    if (query[i][0]) {
      auto [_, l, d, r, u] = query[i];
      cout << kdt.fold({l, r - 1}, {d, u - 1}) << '\n';
    } else {
      auto [_, x, y, w, __] = query[i];
      kdt.set(kdt.get(x, y) + w, x, y);
    }
  }
  return 0;
}