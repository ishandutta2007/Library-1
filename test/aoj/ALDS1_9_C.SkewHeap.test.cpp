#define PROBLEM \
  "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_9_C"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/DataStructure/SkewHeap.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  SkewHeap<int> S;
  string op;
  while (cin >> op && op != "end") {
    if (op[0] == 'i') {
      int k;
      cin >> k;
      S.push(k);
    } else {
      cout << S.pop() << endl;
    }
  }
  return 0;
}