#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2003"
#include <iostream>
#include <algorithm>
#include "src/Geometry/Segment.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 using R= long double;
 int T;
 cin >> T;
 while (T--) {
  Segment<R> AB;
  cin >> AB.p >> AB.q;
  int n;
  cin >> n;
  vector<pair<Point<R>, bool>> cps;
  for (int i= 0; i < n; ++i) {
   Segment<R> s;
   bool o, l;
   cin >> s.p >> s.q >> o >> l;
   auto ps= cross_points(AB, s);
   if (ps.size()) cps.emplace_back(ps[0], 1 ^ o ^ l);
  }
  if (!cps.size()) cout << 0 << '\n';
  else {
   sort(cps.begin(), cps.end());
   bool cur= cps[0].second;
   int ans= 0;
   for (int i= 1; i < cps.size(); ++i)
    if (cur ^ cps[i].second) ++ans, cur= !cur;
   cout << ans << '\n';
  }
 }
 return 0;
}