// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1339
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Math/ModInt_Runtime.hpp"
#include "src/Math/DiscreteLogarithm.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt_Runtime<int>;
 auto f= [](Mint a, Mint b) { return a * b; };
 DiscreteLogarithm log(f, f, [](Mint x) { return x.val(); }, 1e9);
 int T;
 cin >> T;
 while (T--) {
  int N;
  cin >> N;
  while (N % 2 == 0) N/= 2;
  while (N % 5 == 0) N/= 5;
  Mint::set_mod(N);
  cout << log(10, 10, 1) + 1 << '\n';
 }
 return 0;
}