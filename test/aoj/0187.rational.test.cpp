#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Final/0187"
// Rational<long long> だと WA
#include <iostream>
#include <vector>
#include "src/Math/Rational.hpp"
#include "src/Geometry/Polygon.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using namespace geo;
 using R= Rational<__int128_t>;
 for (;;) {
  Segment<R> s1, s2, s3;
  cin >> s1.p >> s1.q;
  if (!sgn(norm(s1.p)) && !sgn(norm(s1.q))) break;
  cin >> s2.p >> s2.q >> s3.p >> s3.q;
  vector<Point<R>> ps;
  if (auto cp= cross_points(s1, s2); cp.size() == 1) ps.push_back(cp[0]);
  if (auto cp= cross_points(s2, s3); cp.size() == 1) ps.push_back(cp[0]);
  if (auto cp= cross_points(s3, s1); cp.size() == 1) ps.push_back(cp[0]);
  Polygon g(ps);
  if (auto a= g.area(); sgn(a - 1'900'000) >= 0) cout << "dai-kichi" << '\n';
  else if (sgn(a - 1'000'000) >= 0) cout << "chu-kichi" << '\n';
  else if (sgn(a - 100'000) >= 0) cout << "kichi" << '\n';
  else if (sgn(a) > 0) cout << "syo-kichi" << '\n';
  else cout << "kyo" << '\n';
 }
 return 0;
}