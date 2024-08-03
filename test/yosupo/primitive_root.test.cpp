// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/primitive_root
#include <iostream>
#include "src/NumberTheory/OrderFp.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int Q;
 cin >> Q;
 while (Q--) {
  long long p;
  cin >> p;
  cout << OrderFp(p).primitive_root() << '\n';
 }
 return 0;
}