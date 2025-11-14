// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/dynamic_tree_vertex_set_path_composite
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
#include <iostream>
#include "src/DataStructure/LinkCutTree.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;

using Mint= ModInt<998244353>;
struct RcompositeQ {
 using T= pair<Mint, Mint>;
 static T op(const T &l, const T &r) { return make_pair(r.first * l.first, r.first * l.second + r.second); }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 LinkCutTree<RcompositeQ> lct(N);
 for (int i= 0; i < N; i++) {
  Mint a, b;
  cin >> a >> b;
  lct.set(i, {a, b});
 }
 for (int i= 0; i < N - 1; i++) {
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
   lct.set(p, {c, d});
  } else {
   int u, v;
   Mint x;
   cin >> u >> v >> x;
   auto [c, d]= lct.prod(u, v);
   cout << c * x + d << '\n';
  }
 }
 return 0;
}
