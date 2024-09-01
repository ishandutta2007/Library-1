// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/2159
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// long long だと TLE(WA?)
#include <iostream>
#include <vector>
#include <set>
#include "src/Math/Rational.hpp"
#include "src/Geometry/Line.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 using R= Rational<__int128_t>;
 int N;
 cin >> N;
 vector<Point<R>> ps(N);
 set<Point<R>> S;
 for (int i= 0; i < N; ++i) cin >> ps[i], S.insert(ps[i]);
 bool isline= true;
 Line m= line_through(ps[0], ps[1]);
 for (int i= 2; i < N; ++i) isline&= m.where(ps[i]) == 0;
 if (isline) return cout << "No" << '\n', 0;
 vector<Line<R>> ls;
 for (int i= 3; i--;)
  for (int j= i + 1; j < N; ++j) ls.emplace_back(bisector(ps[i], ps[j]));
 auto check= [&](const Line<R> &l) {
  int cnt= 0;
  auto ref= reflect(l);
  for (Point p: ps) {
   cnt+= l.where(p) == 0;
   if (!S.count(ref(p))) return false;
  }
  return cnt <= 2;
 };
 bool isok= false;
 for (auto &l: ls) isok|= check(l);
 cout << (isok ? "Yes" : "No") << endl;
 return 0;
}