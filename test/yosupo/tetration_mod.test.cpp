#define PROBLEM "https://judge.yosupo.jp/problem/tetration_mod"
#include <iostream>
#include "src/Math/mod_tetration.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int T;
 cin >> T;
 while (T--) {
  int A, B, M;
  cin >> A >> B >> M;
  cout << mod_tetration(A, B, M) << '\n';
 }
 return 0;
}