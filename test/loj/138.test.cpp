// competitive-verifier: PROBLEM https://loj.ac/p/138
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/Math/AllPurposeEuclid.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<int(1e9 + 7)>;
 using FST= FloorSumTable<Mint, 10, 10>;
 FST::init();

 int T;
 cin >> T;
 while (T--) {
  int n, a, b, c, k1, k2;
  cin >> n >> a >> b >> c >> k1 >> k2;
  cout << FST::solve(n, a, b, c, k1, k2)[k1][k2] << '\n';
 }
 return 0;
}