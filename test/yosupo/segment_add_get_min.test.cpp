#define PROBLEM "https://judge.yosupo.jp/problem/segment_add_get_min"
#include <bits/stdc++.h>
#include "src/DataStructure/LiChaoTree.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, Q;
  cin >> N >> Q;
  LiChaoTree<long long> cht;
  while (N--) {
    long long l, r, a, b;
    cin >> l >> r >> a >> b;
    cht.insert_segment(l, r, a, b);
  }
  while (Q--) {
    bool op;
    cin >> op;
    if (op) {
      long long p;
      cin >> p;
      long long ans = cht.query(p);
      if (ans == cht.get_inf()) {
        cout << "INFINITY" << endl;
      } else {
        cout << ans << endl;
      }
    } else {
      long long l, r, a, b;
      cin >> l >> r >> a >> b;
      cht.insert_segment(l, r, a, b);
    }
  }
  return 0;
}