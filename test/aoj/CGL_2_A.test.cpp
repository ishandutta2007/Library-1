// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/2/CGL_2_A
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Geometry/Line.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 int q;
 cin >> q;
 while (q--) {
  Point<int> lp, lq, mp, mq;
  cin >> lp >> lq >> mp >> mq;
  auto l= line_through(lp, lq), m= line_through(mp, mq);
  if (is_parallel(l, m)) cout << 2 << '\n';
  else if (is_orthogonal(l, m)) cout << 1 << '\n';
  else cout << 0 << '\n';
 }
 return 0;
}