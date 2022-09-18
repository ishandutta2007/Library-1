#define PROBLEM "https://yukicoder.me/problems/no/963"
#include <bits/stdc++.h>
#include "src/Old/ModInt.hpp"
#include "src/Old/FormalPowerSeries.hpp"
#include "src/Old/differential_equation.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<1012924417>;
  using FPS = FormalPowerSeries<Mint>;
  auto F = [](const FPS &f, int deg) {
    auto ret = ((f * f) + 1) / 2;
    return ret.resize(deg), ret;
  };
  auto dF = [](const FPS &f, int deg) {
    return FPS(f.begin(), f.begin() + deg);
  };
  int N;
  cin >> N;
  Mint ans = differential_equation<Mint>(F, dF, 1, N + 1)[N];
  for (int i = N; i >= 1; i--) ans *= i;
  cout << ans * 2 << '\n';
  return 0;
}
