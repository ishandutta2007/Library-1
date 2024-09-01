// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/4/CGL_4_C
// competitive-verifier: ERROR 0.00000001
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <iomanip>
#include "src/Geometry/Convex.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 cout << fixed << setprecision(12);
 using P= Point<long double>;
 int n;
 cin >> n;
 vector<P> ps(n);
 for (int i= 0; i < n; ++i) cin >> ps[i];
 Convex g(ps);
 int q;
 cin >> q;
 while (q--) {
  P lp, lq;
  cin >> lp >> lq;
  cout << g.cut(line_through(lp, lq)).area() << '\n';
 }
 return 0;
}