#define PROBLEM \
  "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_G"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/DataStructure/BinaryIndexedTree_RangeAdd.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, Q;
  cin >> N >> Q;
  BinaryIndexedTree_RangeAdd<long long> bit(N);
  while (Q--) {
    int op, s, t;
    cin >> op >> s >> t;
    s--, t--;
    if (op)
      cout << bit.sum(t + 1) - bit.sum(s) << endl;
    else {
      long long x;
      cin >> x;
      bit.add_range(s, t + 1, x);
    }
  }
}
