// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1573
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// O(√N)
#include <iostream>
#include "src/NumberTheory/enumerate_quotients.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int n, m;
 cin >> n >> m;
 Mint ans= 0;
 for (auto [q, l, r]: enumerate_quotients(n)) {
  long long h= min<long long>(m, r);
  if (l >= h) continue;
  ans+= Mint((h * (h + 1) - l * (l + 1)) / 2) * (q * (q + 3) / 2);
 }
 cout << ans << '\n';
 return 0;
}