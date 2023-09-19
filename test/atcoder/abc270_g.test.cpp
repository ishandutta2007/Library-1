#define PROBLEM "https://atcoder.jp/contests/abc270/tasks/abc270_g"
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
 DiscreteLogarithm log(
     mp, op, [](Mint x) { return x.val(); }, 1e9);
 int T;
 cin >> T;
 while (T--) {
  int P, A, B, S, G;
  cin >> P >> A >> B >> S >> G;
  Mint::set_mod(P);
  cout << log({A, B}, S, G) << '\n';
 }
 return 0;
}