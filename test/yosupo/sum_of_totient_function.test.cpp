#define PROBLEM "https://judge.yosupo.jp/problem/sum_of_totient_function"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "Math/ModInt.hpp"
#include "Math/mobius_inversion.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<998244353>;
  auto g = [](int64_t N, int64_t dummy) { return Mint(N * (N + 1) / 2); };
  auto b = [](int64_t d) { return Mint(d); };
  int64_t N;
  cin >> N;
  Mint ans = mobius_inversion<Mint>(N, N, g, b);
  cout << ans << endl;
  return 0;
}