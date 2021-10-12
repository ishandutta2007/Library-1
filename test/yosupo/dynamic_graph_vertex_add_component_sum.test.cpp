#define PROBLEM \
  "https://judge.yosupo.jp/problem/dynamic_graph_vertex_add_component_sum"
#include <bits/stdc++.h>

#include "src/DataStructure/EulerTourTree.hpp"
#include "src/DataStructure/OnlineDynamicConnectivity.hpp"
using namespace std;

struct Monoid {
  using T = long long;
  static inline T ti() { return 0; }
  static inline T op(const T& lval, const T& rval) { return lval + rval; }
};

int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);
  int N, Q;
  cin >> N >> Q;
  OnlineDynamicConnectivity<Monoid> dicon(N);
  for (int i = 0; i < N; i++) {
    int a;
    cin >> a;
    dicon.set_val(i, a);
  }
  int t;
  int u, v;
  for (int i = 0; i < Q; i++) {
    cin >> t;
    if (t == 0) {
      cin >> u >> v;
      dicon.link(u, v);
    } else if (t == 1) {
      cin >> u >> v;
      dicon.cut(u, v);
    } else if (t == 2) {
      cin >> u >> v;
      dicon.set_val(u, dicon[u] + v);
    } else {
      cin >> v;
      cout << dicon.fold(v) << '\n';
    }
  }
}