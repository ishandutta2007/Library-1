#define PROBLEM \
  "https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_subtree_sum"
#include <bits/stdc++.h>

#include "src/DataStructure/EulerTourTree_Monoid.hpp"
using namespace std;

struct RsumQ {
  using T = long long;
  static T ti() { return 0; }
  static T op(const T &l, const T &r) { return l + r; }
};

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, Q;
  cin >> N >> Q;
  EulerTourTree_Monoid<RsumQ> ett(N);
  for (int i = 0; i < N; i++) {
    long long a;
    cin >> a;
    ett.set_val(i, a);
  }
  for (int i = 0; i < N - 1; i++) {
    int u, v;
    cin >> u >> v;
    ett.link(v, u);
  }
  while (Q--) {
    int op;
    cin >> op;
    if (op == 0) {
      int u, v, w, x;
      cin >> u >> v >> w >> x;
      ett.cut(u, v);
      ett.link(w, x);
    } else if (op == 1) {
      int p;
      long long x;
      cin >> p >> x;
      ett.set_val(p, ett[p] + x);
    } else {
      int v, p;
      cin >> v >> p;
      cout << ett.subtree_fold(v, p) << endl;
    }
  }
  return 0;
}
