// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc212/tasks/abc212_g
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/NumberTheory/ArrayOnDivisors.hpp"
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
 for (auto [d, phi]: a) ans+= phi * d;
 cout << ans << '\n';
 return 0;
}