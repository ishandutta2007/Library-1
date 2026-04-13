// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc212/tasks/abc212_g
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "mylib/algebra/ModInt.hpp"
#include "mylib/number_theory/ArrayOnDivisors.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 long long P;
 cin >> P;
 ArrayOnDivisors<long long, Mint> a(P - 1);
 a.set_totient();
 Mint ans= 1;
 for(auto [d, phi]: a) ans+= phi * d;
 cout << ans << '\n';
 return 0;
}
