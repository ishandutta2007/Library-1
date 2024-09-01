// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/125
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/NumberTheory/ArrayOnDivisors.hpp"
#include "src/Math/FactorialPrecalculation.hpp"
// バーンサイドの補題から求めた
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<int(1e9) + 7>;
 using F= FactorialPrecalculation<Mint>;
 int K;
 cin >> K;
 int tot= 0, g= 0;
 vector<int> C(K);
 for (int i= 0; i < K; ++i) cin >> C[i], tot+= C[i], g= gcd(g, C[i]);
 Mint ans= 0;
 ArrayOnDivisors<int, Mint> A(g);
 A.set_totient();
 for (auto [d, phi]: A) {
  Mint tmp= F::fact(tot / d);
  for (int i= 0; i < K; ++i) tmp*= F::finv(C[i] / d);
  tmp*= phi;
  ans+= tmp;
 }
 ans/= tot;
 cout << ans << '\n';
 return 0;
}