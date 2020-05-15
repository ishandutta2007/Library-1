#define PROBLEM "https://judge.yosupo.jp/problem/line_add_get_min"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "DataStructure/LiChaoTree.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int N, Q;
  cin >> N >> Q;
  LiChaoTree<long long> cht;
  while (N--) {
    long long a, b;
    cin >> a >> b;
    cht.insert_line(a, b);
  }
  while (Q--) {
    bool op;
    cin >> op;
    if (op) {
      long long p;
      cin >> p;
      cout << cht.query(p) << endl;
    } else {
      long long a, b;
      cin >> a >> b;
      cht.insert_line(a, b);
    }
  }
  return 0;
}