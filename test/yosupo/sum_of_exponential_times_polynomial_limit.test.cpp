// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/sum_of_exponential_times_polynomial_limit
// competitive-verifier: TLE 1
// competitive-verifier: MLE 512
/** @see https://min-25.hatenablog.com/entry/2015/04/24/031413
 */
#include <iostream>
#include <vector>
#include "src/Math/FactorialPrecalculation.hpp"
#include "src/Math/ModInt.hpp"
#include "src/NumberTheory/enumerate_primes.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 using F= FactorialPrecalculation<Mint>;
 long long r, d;
 cin >> r >> d;
 vector<Mint> pw(d + 1), y= pow_table<Mint>(d, d);
 pw[0]= 1;
 for (int i= 1; i <= d; i++) pw[i]= pw[i - 1] * r;
 for (int i= 1; i <= d; i++) y[i]= y[i - 1] + pw[i] * y[i];
 Mint ans= 0;
 for (int i= 0; i <= d; i++) {
  Mint tmp= F::nCr(d + 1, i + 1) * pw[d - i] * y[i];
  ans+= (d - i) & 1 ? -tmp : tmp;
 }
 ans/= Mint(1 - r).pow(d + 1);
 cout << ans << '\n';
 return 0;
}