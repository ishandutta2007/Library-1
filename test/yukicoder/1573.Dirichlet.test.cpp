// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1573
// O(N^(2/3)log^(1/3)N))
#include <iostream>
#include "src/NumberTheory/DirichletSeries.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 using DCST= DirichletSeries<Mint>;
 int n, m;
 cin >> n >> m;
 long long M= (long long)m * (m + 1) / 2;
 auto fsum= [&](int x) {
  if (x < m) return (long long)x * (x + 1) / 2;
  return M;
 };
 auto gsum= [&](int x) { return (long long)x * (x + 3) / 2; };
 DCST F(n, fsum), G(n, gsum);
 cout << (F * G).sum() << '\n';
 return 0;
}