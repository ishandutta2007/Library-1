// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/primality_test
#include <iostream>
#include "src/NumberTheory/is_prime.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int Q;
 cin >> Q;
 while (Q--) {
  long long N;
  cin >> N;
  cout << (is_prime(N) ? "Yes" : "No") << '\n';
 }
 return 0;
}