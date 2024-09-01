// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/stirling_number_of_the_first_kind_small_p_large_n
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Math/StirlingNumber.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int T, p;
 cin >> T >> p;
 StirlingNumber SN(p, 1, 0);
 while (T--) {
  long long n, k;
  cin >> n >> k;
  cout << SN.S1(n, k) << '\n';
 }
 return 0;
}