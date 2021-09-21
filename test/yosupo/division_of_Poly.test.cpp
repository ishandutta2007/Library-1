#define PROBLEM "https://judge.yosupo.jp/problem/division_of_polynomials"
#include <bits/stdc++.h>

#include "src/Math/FormalPowerSeries.hpp"
#include "src/Math/ModInt.hpp"
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
  auto [q, r] = f.quorem(g);
  cout << q.size() << " " << r.size() << '\n';
  for (int i = 0; i < (int)q.size(); i++) cout << (i ? " " : "") << q[i];
  cout << '\n';
  for (int i = 0; i < (int)r.size(); i++) cout << (i ? " " : "") << r[i];
  cout << '\n';
  return 0;
}