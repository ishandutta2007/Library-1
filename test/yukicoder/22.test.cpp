#define PROBLEM "https://yukicoder.me/problems/22"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Math/ModInt.hpp"
#include "src/Math/lagrange_interpolation.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<int(1e9 + 9)>;
  const int SIZE = 500 * 6 + 10;
  Mint dp[SIZE] = {1};
  for (int x : {1, 5, 10, 50, 100, 500})
    for (int i = x; i < SIZE; i++) dp[i] += dp[i - x];
  int T;
  cin >> T;
  while (T--) {
    long long M;
    cin >> M;
    vector<Mint> y(6);
    for (int i = 0; i <= 5; i++) y[i] = dp[M % 500 + i * 500];
    cout << lagrange_interpolation<Mint>(y, M / 500) << endl;
  }
  return 0;
}