#define PROBLEM "https://judge.yosupo.jp/problem/binomial_coefficient_prime_mod"
#include <iostream>
#include "src/Math/ModInt_Runtime.hpp"
#include "src/Math/Combination.hpp"
using namespace std;
int main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt_Runtime<int>;
 using Comb= Combination<Mint>;
 int T, m;
 cin >> T >> m;
 Mint::set_mod(m);
 while (T--) {
  int n, k;
  cin >> n >> k;
  cout << Comb::nCr(n, k) << '\n';
 }
 return 0;
}
