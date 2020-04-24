#define PROBLEM \
  "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DPL_5_E"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "Math/Combination.hpp"
#include "Math/ModInt.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  int n, k;
  cin >> n >> k;
  using Mint = ModInt<int(1e9 + 7)>;
  using C = Combination<Mint>;
  C::init(n + k);
  cout << C::C(k, n) << endl;
  return 0;
}
