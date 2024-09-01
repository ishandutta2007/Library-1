// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_subtree_sum
// competitive-verifier: TLE 1
// competitive-verifier: MLE 256
#include <iostream>
#include "src/DataStructure/EulerTourTree.hpp"
using namespace std;
struct RsumQ {
 using T= long long;
 static T ti() { return 0; }
 static T op(T l, T r) { return l + r; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 EulerTourTree<RsumQ> ett(N);
 for (int i= 0; i < N; i++) {
  long long a;
  cin >> a;
  ett.set(i, a);
 }
 for (int i= 0; i < N - 1; i++) {
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
   ett.set(p, ett[p] + x);
  } else {
   int v, p;
   cin >> v >> p;
   cout << ett.prod_subtree(v, p) << '\n';
  }
 }
 return 0;
}
