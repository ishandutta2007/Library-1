// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/unionfind_with_potential_non_commutative_group
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/DataStructure/UnionFind_Potentialized.hpp"
#include "src/Math/ModInt.hpp"
#include "src/LinearAlgebra/Matrix.hpp"
#include "src/Math/Algebra.hpp"
using namespace std;
using Mint= ModInt<998244353>;
using Mat= Matrix<Mint>;
struct M {
 using T= Mat;
 static inline T o= Mat::identity(2);
 static T add(const T &a, const T &b) { return a * b; }
 static T neg(const T &a) { return Mat{{a[1][1], -a[0][1]}, {-a[1][0], a[0][0]}}; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using G= Algebra<M>;
 int N, Q;
 cin >> N >> Q;
 UnionFind_Potentialized<G> uf(N);
 while (Q--) {
  int t, u, v;
  cin >> t >> u >> v;
  if (t) {
   if (uf.connected(u, v)) {
    auto ans= uf.diff(u, v).x;
    cout << ans[0][0] << " " << ans[0][1] << " " << ans[1][0] << " " << ans[1][1] << '\n';
   } else cout << "-1\n";
  } else {
   Mat x(2, 2);
   cin >> x[0][0] >> x[0][1] >> x[1][0] >> x[1][1];
   cout << uf.unite(u, v, x) << '\n';
  }
 }
 return 0;
}