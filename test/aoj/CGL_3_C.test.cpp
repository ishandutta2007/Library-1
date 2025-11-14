// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/3/CGL_3_C
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Geometry/Polygon.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 using P= Point<int>;
 int n;
 cin >> n;
 vector<P> ps(n);
 for (int i= 0; i < n; i++) cin >> ps[i];
 Polygon g(ps);
 int q;
 cin >> q;
 while (q--) {
  P p;
  cin >> p;
  cout << g.where(p) + 1 << '\n';
 }
 return 0;
}