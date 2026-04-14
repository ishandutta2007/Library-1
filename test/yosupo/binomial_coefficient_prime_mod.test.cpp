// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/binomial_coefficient_prime_mod
// competitive-verifier: TLE 1
// competitive-verifier: MLE 256
#include <iostream>
#include "mylib/algebra/ModInt_Runtime.hpp"
#include "mylib/counting/FactorialPrecalculation.hpp"
using namespace std;
int main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt_Runtime<int>;
 using F= FactorialPrecalculation<Mint>;
 int T, m;
 cin >> T >> m;
 Mint::set_mod(m);
 while(T--) {
  int n, k;
  cin >> n >> k;
  cout << F::nCr(n, k) << '\n';
 }
 return 0;
}
