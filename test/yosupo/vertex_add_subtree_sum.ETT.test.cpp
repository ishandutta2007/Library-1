// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/vertex_add_subtree_sum
// competitive-verifier: TLE 2
// competitive-verifier: MLE 256
#include <iostream>
#include <vector>
#include "src/DataStructure/EulerTourTree.hpp"
using namespace std;
struct RSQ {
 using T= long long;
 static T ti() { return 0; }
 static T op(T a, T b) { return a + b; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 EulerTourTree<RSQ> ett(N);
 for (int u= 0, a; u < N; ++u) cin >> a, ett.set(u, a);
 vector<int> p(N, -1);
 for (int i= 1; i < N; ++i) cin >> p[i], ett.link(p[i], i);
 while (Q--) {
  bool op;
  int u;
  cin >> op >> u;
  if (op) {
   cout << ett.prod_subtree(u, p[u]) << '\n';
  } else {
   int x;
   cin >> x;
   ett.set(u, ett.get(u) + x);
  }
 }
 return 0;
}
