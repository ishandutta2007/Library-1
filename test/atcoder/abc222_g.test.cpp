// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc222/tasks/abc222_g
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <array>
#include "src/Math/ModInt_Runtime.hpp"
#include "src/Math/DiscreteLogarithm.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt_Runtime<int>;
 using Aff= array<Mint, 2>;
 auto mp= [](Aff f, Mint x) { return f[0] * x + f[1]; };
 auto op= [](Aff l, Aff r) { return Aff{l[0] * r[0], l[0] * r[1] + l[1]}; };
 DiscreteLogarithm log(mp, op, [](Mint x) { return x.val(); }, 1e8);
 int T;
 cin >> T;
 while (T--) {
  int K;
  cin >> K;
  Mint::set_mod(K);
  int ans= log({10, 2}, 2, 0) + 1;
  cout << (ans ? ans : -1) << '\n';
 }
 return 0;
}