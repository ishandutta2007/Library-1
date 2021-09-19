#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2667"
// パス総和、部分木加算
#include "src/Graph/HeavyLightDecomposition.hpp"
#include "src/DataStructure/BinaryIndexedTree_RangeAdd.hpp"
#include <bits/stdc++.h>
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, Q;
  cin >> N >> Q;
  HeavyLightDecomposition hld(N);
  for (int i = 0; i < N - 1; i++) {
    int a, b;
    cin >> a >> b;
    hld.add_edge(a, b);
  }
  hld.build();
  BinaryIndexedTree_RangeAdd<long long> bit(N);
  auto q = [&](int l, int r) { return bit.sum(r) - bit.sum(l); };
  auto f = [&](long long vl, long long vr) { return vl + vr; };
  while (Q--) {
    int op, x, y;
    cin >> op >> x >> y;
    if (op) {
      auto upd = [&](int l, int r) { bit.add_range(l, r, y); };
      hld.apply_subtree(x, upd, true);
    } else {
      cout << hld.fold_path(x, y, q, f, 0ll, true) << "\n";
    }
  }
  return 0;
}
