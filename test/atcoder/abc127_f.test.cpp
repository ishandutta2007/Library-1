// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc127/tasks/abc127_f
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Optimization/PiecewiseLinearConvex.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int Q;
 cin >> Q;
 PiecewiseLinearConvex<int> f;
 while (Q--) {
  int op;
  cin >> op;
  if (op == 1) {
   int a, b;
   cin >> a >> b;
   f.add_abs(1, a);
   f.add_const(b);
  } else {
   auto [l, r]= f.argmin();
   cout << l << " " << f.min().value() << '\n';
  }
 }
 return 0;
}