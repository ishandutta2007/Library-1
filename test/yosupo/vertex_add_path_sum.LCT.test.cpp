// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/vertex_add_path_sum
// competitive-verifier: TLE 0.5
#include <iostream>
#include "src/DataStructure/LinkCutTree.hpp"
using namespace std;
struct Mono {
 using T= long long;
 static T op(T l, T r) { return l + r; }
 using commute= void;
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 LinkCutTree<Mono> lct(N);
 for (int i= 0, a; i < N; ++i) cin >> a, lct.set(i, a);
 for (int i= 0, u, v; i < N - 1; ++i) cin >> u >> v, lct.link(u, v);
 while (Q--) {
  bool op;
  cin >> op;
  if (op) {
   int u, v;
   cin >> u >> v;
   cout << lct.prod(u, v) << '\n';
  } else {
   int p, x;
   cin >> p >> x;
   lct.mul(p, x);
  }
 }
 return 0;
}