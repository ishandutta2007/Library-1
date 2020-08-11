#define PROBLEM "https://yukicoder.me/problems/3211"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Math/FormalPowerSeries.hpp"
#include "src/Math/ModInt.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<int(1e9 + 7)>;
  using FPS = FormalPowerSeries<Mint>;
  Mint p;
  cin >> p;
  int q_max = 2000000;
  FPS a(q_max);
  a[0] = 0;
  a[1] = 1;
  for (int i = 2; i < q_max; i++) {
    a[i] = p * a[i - 1] + a[i - 2];
  }
  FPS b = a * a;
  int Q;
  cin >> Q;
  while (Q--) {
    int q;
    cin >> q;
    cout << b[q - 2] << endl;
  }
  return 0;
}
