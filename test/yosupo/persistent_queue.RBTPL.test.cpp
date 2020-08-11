#define PROBLEM "https://judge.yosupo.jp/problem/persistent_queue"

// 永続性のverify

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/DataStructure/RedBlackTree_Lazy.hpp"
#include "src/DataStructure/RedBlackTree_PersistentLazy.hpp"
#undef call_from_test

struct Monoid {
  using T = int;
  using E = int;
  static T ti() { return 0; }
  static T f(const T &l, const T &r) { return l; }
  static E ei() { return 0; }
  static T g(const T &l, const E &r) { return l; }
  static E h(const E &l, const E &r) { return l; }
};

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using RBTPL = RedBlackTree_PersistentLazy<Monoid, 1 << 25>;
  int Q;
  cin >> Q;
  vector<RBTPL> S(Q + 1);
  for (int i = 1; i <= Q; i++) {
    int op, t;
    cin >> op >> t;
    S[i] = S[++t];
    if (op) {
      cout << S[i].pop_front() << endl;
    } else {
      int x;
      cin >> x;
      S[i].push_back(x);
    }
  }
  return 0;
}