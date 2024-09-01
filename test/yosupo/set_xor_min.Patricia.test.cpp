// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/set_xor_min
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
#include <iostream>
#include "src/DataStructure/SegmentTree_Patricia.hpp"

// find + xor の verify

using namespace std;
struct RsumQ {
 using T= int;
 static T ti() { return 0; }
 static T op(const T &l, const T &r) { return l + r; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 SegmentTree_Patricia<RsumQ> S;
 auto check= [&](int x) { return x >= 1; };
 int Q;
 cin >> Q;
 while (Q--) {
  int op, x;
  cin >> op >> x;
  if (op == 0) {
   S.set(x, 1);
  } else if (op == 1) {
   S.set(x, 0);
  } else {
   cout << S.find_first(0, check, x) << endl;
  }
 }
 return 0;
}