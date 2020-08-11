#define PROBLEM \
  "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=NTL_2_E"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Math/BigInt.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  BigInt A, B;
  cin >> A >> B;
  cout << A % B << endl;
  return 0;
}