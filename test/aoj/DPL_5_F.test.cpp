#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/5/DPL_5_F"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Math/Combination.hpp"
#include "src/Math/ModInt.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  int n, k;
  cin >> n >> k;
  using Mint = ModInt<int(1e9 + 7)>;
  using C = Combination<Mint>;
  C::init(n + k);
  cout << C::nHr(k, n - k) << endl;
  return 0;
}
