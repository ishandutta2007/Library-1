#define PROBLEM \
  "https://judge.yosupo.jp/problem/stirling_number_of_the_first_kind"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Math/ModInt.hpp"
#include "src/Math/FormalPowerSeries.hpp"
#include "src/Math/fps_sequence.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<998244353>;
  int N;
  cin >> N;
  auto S = stirling_first<Mint>(N).shift(-(N - 1));
  for (int i = 0; i <= N; i++) cout << (i ? " " : "") << S[i];
  cout << endl;
  return 0;
}