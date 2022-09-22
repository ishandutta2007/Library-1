#define PROBLEM "https://judge.yosupo.jp/problem/inv_of_polynomials"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/FFT/Polynomial.hpp"
#include "src/FFT/extgcd.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = StaticModInt<998244353>;
  int N, M;
  cin >> N >> M;
  Polynomial<Mint> f(N), g(M), x, y;
  for (int i = 0; i < N; i++) cin >> f[i];
  for (int i = 0; i < M; i++) cin >> g[i];
  auto d = extgcd(f, g, x, y);
  if (d.deg() != 0) {
    cout << -1 << '\n';
  } else if (x.deg() == -1) {
    cout << 0 << '\n';
  } else {
    cout << x.size() << '\n';
    x /= d[0];
    for (int i = 0, ed = x.size(); i < ed; i++)
      cout << x[i] << " \n"[i == ed - 1];
  }
  return 0;
}