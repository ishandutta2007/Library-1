#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=0168"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "Math/FormalPowerSeries.hpp"
#include "Math/Kitamasa.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  int n;
  int mod = 1e9 + 7;
  vector<R> c = {1, 1, 1}, a = {1, 1, 2};
  while (cin >> n && n) {
    cout << (kitamasa(c, a, n, mod) - 1) / 3650 + 1 << "\n";
  }
  cout << flush;
  return 0;
}
