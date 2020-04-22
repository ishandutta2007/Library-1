#define PROBLEM \
  "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_14_D"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "String/SuffixArray.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  string t;
  cin >> t;
  SuffixArray sa(t);
  int Q;
  cin >> Q;
  while (Q--) {
    string p;
    cin >> p;
    cout << (sa.count(p) > 0) << "\n";
  }
  cout << flush;
  return 0;
}