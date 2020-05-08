#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1508"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "DataStructure/RedBlackTree.hpp"
#undef call_from_test

struct RminQ {
  using T = int;
  static T ti() { return INT_MAX; }
  static T f(const T &l, const T &r) { return min(l, r); }
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using RBT = RedBlackTree<RminQ>;
  int N, Q;
  cin >> N >> Q;
  vector<int> A(N);
  for (int i = 0; i < N; i++) cin >> A[i];
  RBT rbt(A);
  while (Q--) {
    int x, y, z;
    cin >> x >> y >> z;
    if (x == 0) {
      RBT a, b, c;
      tie(a, b, c) = rbt.split3(y, z + 1);
      auto val = b.pop_back();
      b.push_front(val);
      rbt = a + b + c;
    } else if (x == 1) {
      cout << rbt.query(y, z + 1) << endl;
    } else {
      rbt.set_val(y, z);
    }
  }
}