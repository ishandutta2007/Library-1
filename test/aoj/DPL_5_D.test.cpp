// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/5/DPL_5_D
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Math/FactorialPrecalculation.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int n, k;
 cin >> n >> k;
 using Mint= ModInt<int(1e9 + 7)>;
 using F= FactorialPrecalculation<Mint>;
 cout << F::nHr(k, n) << '\n';
 return 0;
}
