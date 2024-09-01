// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/binomial_coefficient
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Math/BinomialCoefficient.hpp"
using namespace std;
int main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int T, m;
 cin >> T >> m;
 const BinomialCoefficient binom(m);
 while (T--) {
  long long n, k;
  cin >> n >> k;
  cout << binom.nCr(n, k) << '\n';
 }
 return 0;
}
