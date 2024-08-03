// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/2342

// 双対のverify

#include <iostream>
#include <tuple>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Graph/Graph.hpp"
#include "src/Graph/HeavyLightDecomposition.hpp"
#include "src/DataStructure/KDTree.hpp"
using namespace std;
using Mint= ModInt<998244353>;
struct RaffineQ {
 using T= Mint;
 using E= array<Mint, 2>;
 static void mp(T &v, const E &x) { v= x[0] * v + x[1]; }
 static void cp(E &p, const E &n) { p[0]*= n[0], p[1]*= n[0], p[1]+= n[1]; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 Graph g(N, N - 1);
 for (int i= 0; i < N - 1; ++i) cin >> g[i], --g[i];
 HeavyLightDecomposition tree(g, 0);
 vector<tuple<int, int, Mint>> xy;
 for (int v= 0; v < N; ++v) {
  Mint X;
  cin >> X;
  xy.emplace_back(tree.to_seq(v), tree.depth(v), X);
 }
 KDTree<int, 2, RaffineQ> kdt(xy);
 while (Q--) {
  int t;
  cin >> t;
  if (t == 1) {
   int V;
   cin >> V, --V;
   cout << kdt.get(tree.to_seq(V), tree.depth(V)) << "\n";
  } else if (t == 2) {
   int V, K, C, D;
   cin >> V >> K >> C >> D, --V;
   for (int i= 0; i <= K; ++i) {
    int p= tree.parent(V);
    auto [l, r]= tree.subtree(V);
    int d= tree.depth(V);
    if (p == -1) {
     kdt.apply_cuboid(l, r - 1, d, d + K - i, {C, D});
     break;
    }
    kdt.apply_cuboid(l, r - 1, d + K - i - 1, d + K - i, {C, D});
    V= p;
   }
  } else if (t == 3) {
   int V, C, D;
   cin >> V >> C >> D, --V;
   auto [l, r]= tree.subtree(V);
   kdt.apply_cuboid(l, r - 1, 0, N, {C, D});
  } else {
   int U, V, C, D;
   cin >> U >> V >> C >> D, --U, --V;
   for (auto [a, b]: tree.path(U, V)) a < b ? kdt.apply_cuboid(a, b, 0, N, {C, D}) : kdt.apply_cuboid(b, a, 0, N, {C, D});
  }
 }
 return 0;
}