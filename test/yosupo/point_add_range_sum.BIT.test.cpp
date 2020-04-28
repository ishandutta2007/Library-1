#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "DataStructure/BinaryIndexedTree.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, Q;
  cin >> N >> Q;
  BinaryIndexedTree bit(N);
  for (int i = 0; i < N; i++) {
    long long a;
    cin >> a;
    bit.add(i, a);
  }
  while (Q--) {
    int t, a, b;
    cin >> t >> a >> b;
    if (t)
      cout << bit[b] - bit[a] << endl;
    else
      bit.add(a, b);
  }
}
