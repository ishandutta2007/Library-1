#define PROBLEM "https://judge.yosupo.jp/problem/inv_of_polynomials"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/FormalPowerSeries.hpp"
#include "src/Math/extgcd.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<998244353>;
  using Poly = FormalPowerSeries<Mint>;
  int N, M;
  cin >> N >> M;
  Poly f(N), g(M), x, y;
  for (int i = 0; i < N; i++) cin >> f[i];
  for (int i = 0; i < M; i++) cin >> g[i];
  Poly d = extgcd(f, g, x, y);
  if (d.deg() != 0) {
    cout << -1 << '\n';
  } else if (x.deg() == -1) {
    cout << 0 << '\n';
  } else {
    cout << x.size() << '\n';
    x /= d[0];
    for (size_t i = 0; i < x.size(); i++) cout << (i ? " " : "") << x[i];
    cout << '\n';
  }
  return 0;
}