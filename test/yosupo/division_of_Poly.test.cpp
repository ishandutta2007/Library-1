// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/division_of_polynomials
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/FFT/Polynomial.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N, M;
 cin >> N >> M;
 Polynomial<Mint> f(N), g(M), x, y;
 for (int i= 0; i < N; i++) cin >> f[i];
 for (int i= 0; i < M; i++) cin >> g[i];
 auto [q, r]= f.quorem(g);
 cout << q.deg() + 1 << " " << r.deg() + 1 << '\n';
 if (q.deg() != -1)
  for (int i= 0, ed= q.size(); i < ed; i++) cout << q[i] << " \n"[i == ed - 1];
 if (r.deg() != -1)
  for (int i= 0, ed= r.size(); i < ed; i++) cout << r[i] << " \n"[i == ed - 1];
 return 0;
}