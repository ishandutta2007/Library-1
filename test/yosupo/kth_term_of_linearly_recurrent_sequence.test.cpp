#define PROBLEM \
  "https://judge.yosupo.jp/problem/kth_term_of_linearly_recurrent_sequence"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/FormalPowerSeries.hpp"
#include "src/Math/kitamasa.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<998244353>;
  long long d, k;
  cin >> d >> k;
  vector<Mint> a(d), c(d);
  for (int i = 0; i < d; i++) cin >> a[i];
  for (int i = 0; i < d; i++) cin >> c[i];
  cout << kitamasa(c, a, k) << '\n';
  return 0;
}