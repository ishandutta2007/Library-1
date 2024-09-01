// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Final/0445
// competitive-verifier: ERROR 0.00000001
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "src/Geometry/Polygon.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 int N;
 cin >> N;
 vector<Point<long long>> ps(N);
 for (int i= 0; i < N; ++i) cin >> ps[i];
 auto area= [&](int i, int j, int k) { return Polygon<long long>({ps[i % N], ps[j % N], ps[k % N]}).area2(); };
 Polygon g(ps);
 auto all= g.area2(), val= area(0, 1, 2), ans= abs(all - val * 2);
 for (int l= 0, r= 3; r < N * 2; ++r) {
  val+= area(l, r - 1, r);
  for (; r - l >= 3 && val > all - val; ++l) val-= area(l, l + 1, r);
  ans= min(ans, abs(all - val * 2));
 }
 cout << fixed << setprecision(1) << 0.5 * ans << '\n';
 return 0;
}