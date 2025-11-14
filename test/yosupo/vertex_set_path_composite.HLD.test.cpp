// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/vertex_set_path_composite
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include "src/Math/ModInt.hpp"
#include "src/Graph/Graph.hpp"
#include "src/Graph/HeavyLightDecomposition.hpp"
#include "src/DataStructure/SegmentTree.hpp"
using namespace std;
using Mint= ModInt<998244353>;
struct Mono {
 using T= array<Mint, 2>;
 static T ti() { return {Mint(1), Mint()}; }
 static T op(const T &l, const T &r) { return {l[0] * r[0], l[1] * r[0] + r[1]}; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 Mint a[N], b[N];
 for (int i= 0; i < N; ++i) cin >> a[i] >> b[i];
 Graph g(N, N - 1);
 for (int i= 0; i < N - 1; ++i) cin >> g[i];
 HeavyLightDecomposition tree(g, 0);
 vector<typename Mono::T> vec(N);
 for (int v= 0; v < N; ++v) vec[tree.to_seq(v)]= {a[v], b[v]};
 SegmentTree<Mono> seg1(vec);
 reverse(vec.begin(), vec.end());
 SegmentTree<Mono> seg2(vec);
 while (Q--) {
  bool op;
  cin >> op;
  if (op) {
   int u, v;
   Mint x;
   cin >> u >> v >> x;
   auto f= Mono::ti();
   for (auto [s, t]: tree.path(u, v)) f= Mono::op(f, s < t ? seg1.prod(s, t + 1) : seg2.prod(N - s - 1, N - t));
   cout << f[0] * x + f[1] << '\n';
  } else {
   int p;
   Mint c, d;
   cin >> p >> c >> d;
   int i= tree.to_seq(p);
   seg1.set(i, {c, d});
   seg2.set(N - i - 1, {c, d});
  }
 }
 return 0;
}