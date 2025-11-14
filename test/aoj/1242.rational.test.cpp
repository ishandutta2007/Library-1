// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/1242
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
// 浮動小数点数 + (std の ceil, floor) だと WA
#include <iostream>
#include <algorithm>
#include <vector>
#include "src/Math/Rational.hpp"
#include "src/Geometry/Polygon.hpp"
#include "src/DataStructure/RangeSet.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 using Q= Rational<long long, false>;
 for (int N; cin >> N && N;) {
  vector<Point<Q>> ps(N);
  for (int i= 0; i < N; ++i) cin >> ps[i];
  Polygon<Q> g(ps);
  int ans= 0;
  Point<Q> h{1, 0};
  for (int y= -2001; y < 2001; ++y) {
   Line l({0, (Q)y}, h), m({0, (Q)y + 1}, h);
   vector<Q> x_d, x_u;
   for (auto &e: g.edges()) {
    auto cp_d= cross_points(l, e);
    auto cp_u= cross_points(m, e);
    if (cp_d.size() != 1 || cp_u.size() != 1) continue;
    x_d.push_back(cp_d[0].x), x_u.push_back(cp_u[0].x);
   }
   sort(x_d.begin(), x_d.end()), sort(x_u.begin(), x_u.end());
   RangeSet<int> rs;
   int n= x_d.size(), p= -2001;
   for (int i= 0; i < n; ++i) {
    if (i & 1) {
     int q= ceil(max(x_d[i], x_u[i]));
     if (p < q) rs.insert(p, q - 1);
    } else p= floor(min(x_d[i], x_u[i]));
   }
   for (auto [l, r]: rs) ans+= r - l + 1;
  }
  cout << ans << '\n';
 }
 return 0;
}