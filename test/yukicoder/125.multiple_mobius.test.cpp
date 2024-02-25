#define PROBLEM "https://yukicoder.me/problems/no/125"
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/NumberTheory/ArrayOnDivisors.hpp"
#include "src/Math/FactorialPrecalculation.hpp"
// 倍数メビウス
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
 ArrayOnDivisors<int, Mint> A(g);
 for (auto &[d, v]: A) {
  v= F::fact(tot / d);
  for (int i= 0; i < K; ++i) v*= F::finv(C[i] / d);
 }
 A.multiple_mobius();
 Mint ans= 0;
 for (auto [d, v]: A) ans+= v * d;
 ans/= tot;
 cout << ans << '\n';
 return 0;
}