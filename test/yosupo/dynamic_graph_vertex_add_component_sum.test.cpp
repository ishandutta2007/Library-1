#define PROBLEM "https://judge.yosupo.jp/problem/dynamic_graph_vertex_add_component_sum"
#include <iostream>
#include "src/DataStructure/OnlineDynamicConnectivity.hpp"
using namespace std;
struct Monoid {
 using T= long long;
 static inline T ti() { return 0; }
 static inline T op(T lval, T rval) { return lval + rval; }
};
int main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 OnlineDynamicConnectivity<Monoid> dicon(N);
 for (int i= 0; i < N; i++) {
  int a;
  cin >> a;
  dicon.set(i, a);
 }
 for (int i= 0; i < Q; i++) {
  int t;
  int u, v;
  cin >> t;
  if (t == 0) {
   cin >> u >> v;
   dicon.link(u, v);
  } else if (t == 1) {
   cin >> u >> v;
   dicon.cut(u, v);
  } else if (t == 2) {
   cin >> u >> v;
   dicon.set(u, dicon[u] + v);
  } else {
   cin >> v;
   cout << dicon.fold(v) << '\n';
  }
 }
}