// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/persistent_queue
// competitive-verifier: TLE 2

// 永続性のverify

#include <iostream>
#include <vector>
#include "src/DataStructure/WeightBalancedTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int Q;
 cin >> Q;
 vector<WeightBalancedTree<int, false, true>> S(Q + 1);
 for (int i= 1; i <= Q; ++i) {
  int op, t;
  cin >> op >> t;
  S[i]= S[++t];
  if (op) {
   cout << S[i].pop_front() << '\n';
  } else {
   int x;
   cin >> x;
   S[i].push_back(x);
  }
 }
 return 0;
}