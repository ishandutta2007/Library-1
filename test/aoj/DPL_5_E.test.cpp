#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/5/DPL_5_E"
#include <bits/stdc++.h>
#include "src/Math/Combination.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  int n, k;
  cin >> n >> k;
  using Mint = StaticModInt<int(1e9 + 7)>;
  using C = Combination<Mint>;
  cout << C::nCr(k, n) << endl;
  return 0;
}
