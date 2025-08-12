// competitive-verifier: PROBLEM https://loj.ac/p/6714
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/NumberTheory/DirichletSeries.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 uint64_t n;
 cin >> n;
 cout << (Mint(1) / (Mint(2) - get_1<Mint>(n))).sum() << '\n';
 return 0;
}