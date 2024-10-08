// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/650
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/LinearAlgebra/Matrix.hpp"
#include "src/Graph/Graph.hpp"
#include "src/Graph/HeavyLightDecomposition.hpp"
#include "src/DataStructure/SegmentTree.hpp"
using namespace std;

using Mint= ModInt<int(1e9) + 7>;
using Mat= Matrix<Mint>;
Mat I= Mat::identity(2);
struct Monoid {
 using T= Mat;
 static T ti() { return I; }
 static T op(const T &l, const T &r) { return l * r; }
};
int main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int n;
 cin >> n;
 SegmentTree<Monoid> seg(n, I);
 Graph g(n, n - 1);
 for (int e= 0; e < n - 1; ++e) cin >> g[e];
 HeavyLightDecomposition hld(g);
 for (auto &[u, v]: g)
  if (hld.in_subtree(u, v)) swap(u, v);

 int q;
 cin >> q;
 while (q--) {
  char t;
  cin >> t;
  if (t == 'x') {
   int e;
   cin >> e;
   Mat X(2, 2);
   cin >> X[0][0] >> X[0][1] >> X[1][0] >> X[1][1];
   int i= hld.to_seq(g[e].second);
   seg.set(i, X);
  } else {
   int u, v;
   cin >> u >> v;
   Mat ans= I;
   for (auto [l, r]: hld.path(u, v, true)) ans*= seg.prod(l, r + 1);
   cout << ans[0][0] << " " << ans[0][1] << " " << ans[1][0] << " " << ans[1][1] << '\n';
  }
 }
 return 0;
}