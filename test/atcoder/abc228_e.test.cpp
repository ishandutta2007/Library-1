// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc228/tasks/abc228_e
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Math/ModInt_Exp.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt_Exp<998244353>;
 Mint n, k, m;
 cin >> n >> k >> m;
 cout << m.pow(k.pow(n)) << '\n';
 return 0;
}