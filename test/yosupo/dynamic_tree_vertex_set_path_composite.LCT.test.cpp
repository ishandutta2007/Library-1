#define PROBLEM \
  "https://judge.yosupo.jp/problem/dynamic_tree_vertex_set_path_composite"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "DataStructure/LinkCutTree_Monoid.hpp"
#include "Math/ModInt.hpp"
#undef call_from_test

using Mint = ModInt<998244353>;
struct RcompositeQ {
  using T = pair<Mint, Mint>;
  static T ti() { return make_pair(Mint(1), Mint(0)); }
  static T f(const T &l, const T &r) {
    return make_pair(r.first * l.first, r.first * l.second + r.second);
  }
};

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, Q;
  cin >> N >> Q;
  LinkCutTree_Monoid<RcompositeQ> lct(N);
  for (int i = 0; i < N; i++) {
    Mint a, b;
    cin >> a >> b;
    lct.set_val(i, {a, b});
  }
  for (int i = 0; i < N - 1; i++) {
    int u, v;
    cin >> u >> v;
    lct.link(u, v);
  }
  while (Q--) {
    int op;
    cin >> op;
    if (op == 0) {
      int u, v, w, x;
      cin >> u >> v >> w >> x;
      lct.cut(u, v);
      lct.link(w, x);
    } else if (op == 1) {
      int p;
      Mint c, d;
      cin >> p >> c >> d;
      lct.set_val(p, {c, d});
    } else {
      int u, v;
      Mint x;
      cin >> u >> v >> x;
      auto ans = lct.query(u, v);
      cout << ans.first * x + ans.second << endl;
    }
  }
  return 0;
}
