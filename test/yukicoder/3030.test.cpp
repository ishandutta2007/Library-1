// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/3030
// competitive-verifier: TLE 0.5
#include <iostream>
#include "src/NumberTheory/is_prime.hpp"

using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int n;
 cin >> n;
 while (n--) {
  long long x;
  cin >> x;
  cout << x << " " << is_prime(x) << '\n';
 }
 return 0;
}