#define PROBLEM \
  "https://judge.yosupo.jp/problem/sum_of_exponential_times_polynomial_limit"

/** @see https://en.m.wikipedia.org/wiki/Eulerian_number
 */

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Math/FormalPowerSeries.hpp"
#include "src/Math/fps_sequence.hpp"
#include "src/Math/ModInt.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<998244353>;
  int r, d;
  cin >> r >> d;
  auto A = eulerian<Mint>(d);
  Mint ans = 0, rpw = r;
  for (int i = 0; i < d; i++, rpw *= r) ans += A[i] * rpw;
  ans /= Mint(1 - r).pow(d + 1);
  cout << ans << endl;
  return 0;
}