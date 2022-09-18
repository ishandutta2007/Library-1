#define PROBLEM "https://atcoder.jp/contests/agc038/tasks/agc038_c"

#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/Sieve.hpp"
using namespace std;

// O(MAX_A log log MAX_A)

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using Mint = StaticModInt<998244353>;
  const int MAX_A = 1e6;
  int N;
  cin >> N;
  vector<Mint> c(MAX_A + 1, 0);
  Mint sum = 0;
  for (int i = 0, A; i < N; i++) cin >> A, c[A] += A, sum += A;
  auto tmp = Sieve<>::gcd_conv(c, c);
  Mint sum2 = 0;
  for (int d = 1; d <= MAX_A; d++) sum2 += tmp[d] / d;
  cout << (sum2 - sum) / 2 << endl;
  return 0;
}