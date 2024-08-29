// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/4/CGL_4_B
// competitive-verifier: ERROR 0.00000001
// competitive-verifier: TLE 0.5
#include <iostream>
#include <iomanip>
#include "src/Geometry/Convex.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 int n;
 cin >> n;
 vector<Point<long double>> ps(n);
 for (int i= 0; i < n; ++i) cin >> ps[i];
 Convex g(ps);
 cout << fixed << setprecision(12) << g.diameter() << '\n';
 return 0;
}