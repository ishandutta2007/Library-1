// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc236/tasks/abc236_Ex
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// https://atcoder.jp/contests/abc236/tasks/abc236_h
#include <iostream>
#include <numeric>
#include "src/Math/ModInt.hpp"
#include "src/Math/FactorialPrecalculation.hpp"
#include "src/Math/set_power_series.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 using F= FactorialPrecalculation<Mint>;
 long long N, M;
 cin >> N >> M;
 long long S= 1 << N;
 long long D[N], f[S];
 for (int i= 0; i < N; ++i) cin >> D[i];
 f[0]= 1;
 for (int i= 0, I= 1; i < N; ++i, I<<= 1)
  for (int s= 0; s < I; ++s) {
   long long g= gcd(f[s], D[i]), a= D[i] / g;
   f[s | I]= a <= M / f[s] ? a * f[s] : M + 1;
  }
 vector<Mint> g(S);
 for (int s= 1; s < S; ++s) {
  int n= __builtin_popcount(s);
  Mint tmp= F::fact(n - 1) * (M / f[s]);
  g[s]= n & 1 ? tmp : -tmp;
 }
 cout << sps::exp(g)[S - 1] << '\n';
 return 0;
}