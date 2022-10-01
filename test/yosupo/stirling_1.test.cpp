#define PROBLEM \
  "https://judge.yosupo.jp/problem/stirling_number_of_the_first_kind"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/FFT/fps_sequences.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = StaticModInt<998244353>;
  int N;
  cin >> N;
  auto ans = stirling_first<Mint>(N);
  for (int i = 0; i <= N; i++) cout << ans[N - i] << " \n"[i == N];
  return 0;
}