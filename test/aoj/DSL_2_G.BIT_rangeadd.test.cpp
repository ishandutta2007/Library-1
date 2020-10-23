#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/2/DSL_2_G"
#include <bits/stdc++.h>
#include "src/DataStructure/BinaryIndexedTree_RangeAdd.hpp"
using namespace std;

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
