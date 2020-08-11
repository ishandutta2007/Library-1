#define PROBLEM "https://judge.yosupo.jp/problem/lca"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/DataStructure/LinkCutTree.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, Q;
  cin >> N >> Q;
  LinkCutTree lct(N);
  for (int i = 1; i < N; i++) {
    int p;
    cin >> p;
    lct.link(i, p);
  }
  lct.evert(0);
  while (Q--) {
    int u, v;
    cin >> u >> v;
    cout << lct.lca(u, v) << endl;
  }
  return 0;
}
