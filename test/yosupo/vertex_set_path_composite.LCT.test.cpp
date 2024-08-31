// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/vertex_set_path_composite
// competitive-verifier: TLE 0.5
#include <iostream>
#include <algorithm>
#include <array>
#include "src/Math/ModInt.hpp"
#include "src/DataStructure/LinkCutTree.hpp"
using namespace std;
using Mint= ModInt<998244353>;
struct Mono {
 using T= array<Mint, 2>;
 static T op(const T &l, const T &r) { return {l[0] * r[0], l[1] * r[0] + r[1]}; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 LinkCutTree<Mono> lct(N);
 for (int i= 0; i < N; ++i) {
  Mint a, b;
  cin >> a >> b;
  lct.set(i, {a, b});
 }
 for (int i= 0, u, v; i < N - 1; ++i) cin >> u >> v, lct.link(u, v);

 while (Q--) {
  bool op;
  cin >> op;
  if (op) {
   int u, v;
   Mint x;
   cin >> u >> v >> x;
   auto [a, b]= lct.prod(u, v);
   cout << a * x + b << '\n';
  } else {
   int p;
   Mint c, d;
   cin >> p >> c >> d;
   lct.set(p, {c, d});
  }
 }
 return 0;
}