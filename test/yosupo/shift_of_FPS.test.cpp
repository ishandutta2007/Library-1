#define PROBLEM "https://judge.yosupo.jp/problem/polynomial_taylor_shift"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/FFT/Polynomial.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = StaticModInt<998244353>;
  using Poly = Polynomial<Mint>;
  int N, c;
  cin >> N >> c;
  Poly a(N);
  for (int i = 0; i < N; i++) cin >> a[i];
  auto x = Poly::x();
  auto b = a(x + c);
  b.resize(N);
  for (int i = 0; i < N; i++) cout << b[i] << " \n"[i == N - 1];
  return 0;
}