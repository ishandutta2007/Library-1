#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/6/NTL/2/NTL_2_B"

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
  cout << A - B << endl;
  return 0;
}