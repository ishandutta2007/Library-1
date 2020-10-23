#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=0168"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/FormalPowerSeries.hpp"
#include "src/Math/kitamasa.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  int n;
  using Mint = ModInt<int(1e9 + 7)>;
  vector<Mint> c = {1, 1, 1}, a = {1, 1, 2};
  while (cin >> n && n) {
    cout << (kitamasa(c, a, n).get() - 1) / 3650 + 1 << "\n";
  }
  cout << flush;
  return 0;
}
