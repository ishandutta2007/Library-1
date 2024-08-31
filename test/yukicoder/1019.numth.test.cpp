// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1019
// competitive-verifier: TLE 0.5
#include <iostream>
#include <cmath>
#include "src/Math/ModInt.hpp"
#include "src/NumberTheory/tables.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 auto F= [](uint64_t n) {
  Mint S= 0;
  for (uint64_t x= sqrt(n); x; --x) S+= x * (1 + 2 * (uint64_t)sqrt(n - x * x));
  return S;
 };
 uint64_t N;
 cin >> N;
 uint64_t sqrtN= sqrt(N);
 auto mu= mobius_table(sqrtN);
 Mint ans= 0;
 for (int d= 1; d <= sqrtN; d++) ans+= F(N / d / d) * d * mu[d];
 cout << ans * 24 - 16 << '\n';
 return 0;
}
