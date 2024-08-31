// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/2578
// competitive-verifier: TLE 0.5
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/NumberTheory/ArrayOnDivisors.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 long long T, m;
 cin >> T >> m;
 ArrayOnDivisors<long long, Mint> X(m);
 while (T--) {
  int n;
  Mint B, C, D;
  cin >> n >> B >> C >> D;
  for (auto &[d, v]: X) v= 1;
  Mint W= B;
  for (long long a; n--;) {
   cin >> a;
   if (m % a == 0) X[a]*= W + 1;
   W= W * C + D;
  }
  X.divisor_zeta([](Mint &a, Mint b) { a*= b; });
  for (auto &[d, v]: X) v-= 1;
  X.divisor_mobius();
  cout << X[m] << '\n';
 }
 return 0;
}