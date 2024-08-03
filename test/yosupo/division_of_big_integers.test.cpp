// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/division_of_big_integers
#include <iostream>
#include "src/FFT/BigInt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int T;
 cin >> T;
 while (T--) {
  BigInt A, B;
  cin >> A >> B;
  auto q= A / B;
  auto r= A - q * B;
  cout << q << " " << r << '\n';
 }
 return 0;
}