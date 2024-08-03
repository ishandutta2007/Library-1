// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/2009
#include <iostream>
#include <iomanip>
#include <vector>
#include "src/Math/Rational.hpp"
#include "src/Geometry/Segment.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 using R= Rational<long long, false>;
 for (int N; cin >> N && N;) {
  int ans= 1;
  vector<Segment<R>> ss;
  for (int i= 0; i < N; i++) {
   Segment<R> s;
   cin >> s.p >> s.q;
   ++ans;
   vector<Point<R>> ps{s.p, s.q};
   for (auto &t: ss) {
    auto cp= cross_points(s, t);
    if (!cp.size()) continue;
    bool isok= true;
    for (auto &p: ps) isok&= cp[0] != p;
    if (isok) ++ans, ps.push_back(cp[0]);
   }
   ss.push_back(s);
  }
  cout << ans << '\n';
 }
 return 0;
}