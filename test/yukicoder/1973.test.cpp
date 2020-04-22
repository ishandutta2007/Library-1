#define PROBLEM "https://yukicoder.me/problems/1973"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "Math/FormalPowerSeries.hpp"
#include "Math/Kitamasa.hpp"
#include "Math/ModInt.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int Q;
  cin >> Q;
  while (Q--) {
    long long n;
    cin >> n;
    cout << kitamasa<ModInt<17>>({1, 1, 1, 1}, {0, 0, 0, 1}, n - 1) << "\n";
  }
  cout << flush;
  return 0;
}