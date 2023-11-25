#define PROBLEM "https://yukicoder.me/problems/no/1260"
// O(√N)
#include <iostream>
#include "src/NumberTheory/enumerate_quotients.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<int(1e9 + 7)>;
 long long N;
 cin >> N;
 Mint ans= 0;
 for (auto [q, l, r]: enumerate_quotients(N)) {
  if (l == 0) continue;
  if (r - l == 1)
   for (long long n= N; n; n/= r) ans+= n % r;
  else ans+= Mint(q + N) * (r - l) - (Mint(r + 1) * r - Mint(l + 1) * l) * q / 2;
 }
 cout << ans << '\n';
 return 0;
}