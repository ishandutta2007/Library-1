#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/all/DSL_2_C"
#include <bits/stdc++.h>
#include "src/DataStructure/KDTree.hpp"
using namespace std;

struct IDs {
  using T = vector<int>;
  static T ti() { return {}; }
  static T op(T l, T r) {
    if (l.size() < r.size()) l.swap(r);
    for (int x : r) l.push_back(x);
    return l;
  }
};
signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  int n;
  cin >> n;
  using KDT = KDTree<2, int, IDs>;
  vector<typename KDT::PosVal> v(n);
  for (int i = 0; i < n; i++) {
    int x, y;
    cin >> x >> y;
    v[i].first = {x, y}, v[i].second = {i};
  }
  KDT kdt(v);
  int q;
  cin >> q;
  while (q--) {
    int sx, tx, sy, ty;
    cin >> sx >> tx >> sy >> ty;
    auto ans = kdt.fold({sx, tx}, {sy, ty});
    sort(ans.begin(), ans.end());
    for (auto x : ans) cout << x << '\n';
    cout << '\n';
  }
  return 0;
}