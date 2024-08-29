// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/sqrt_mod
// competitive-verifier: TLE 0.5
#include <iostream>
#include "src/Math/mod_sqrt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int T;
 cin >> T;
 while (T--) {
  int Y, P;
  cin >> Y >> P;
  cout << mod_sqrt(Y, P) << '\n';
 }
 return 0;
}