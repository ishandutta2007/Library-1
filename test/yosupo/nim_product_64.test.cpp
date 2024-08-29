// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/nim_product_64
// competitive-verifier: TLE 0.5
#include <iostream>
#include "src/Math/Nimber.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 Nimber::init();
 int T;
 cin >> T;
 for (int i= 0; i < T; ++i) {
  Nimber a, b;
  cin >> a >> b;
  cout << a * b << '\n';
 }
 return 0;
}