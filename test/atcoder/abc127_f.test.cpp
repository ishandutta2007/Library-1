#define PROBLEM "https://atcoder.jp/contests/abc127/tasks/abc127_f"
#include <iostream>
#include "src/Optimization/PiecewiseLinearConvexfunction.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int Q;
 cin >> Q;
 PiecewiseLinearConvexfunction f;
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
   cout << l << " " << f(l) << '\n';
  }
 }
 return 0;
}