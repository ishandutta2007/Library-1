// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/discrete_logarithm_mod
#include <iostream>
#include "src/Math/ModInt_Runtime.hpp"
#include "src/Math/DiscreteLogarithm.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt_Runtime<int>;
 auto f= [](Mint a, Mint b) { return a * b; };
 DiscreteLogarithm log(
     f, f, [](Mint x) { return x.val(); }, 1e9);
 int T;
 cin >> T;
 while (T--) {
  int X, Y, M;
  cin >> X >> Y >> M;
  Mint::set_mod(M);
  cout << log(X, 1, Y) << '\n';
 }
 return 0;
}