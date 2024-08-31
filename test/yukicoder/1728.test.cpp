// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1728
// competitive-verifier: TLE 0.5
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/NumberTheory/ArrayOnDivisors.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<int(1e9) + 7>;
 int T;
 cin >> T;
 while (T--) {
  int N;
  Mint C;
  cin >> N >> C;
  ArrayOnDivisors<int, int> A(N);
  A.set_totient();
  Mint ans= 0;
  for (auto [d, phi]: A) ans+= C.pow(2 * N / d) * phi;
  ans+= C.pow(N) * N, ans/= N + N;
  cout << ans << '\n';
 }
 return 0;
}