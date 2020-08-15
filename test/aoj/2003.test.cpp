#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2003"
#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Geometry/!geometry_temp.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using namespace geometry;
  int T;
  cin >> T;
  while (T--) {
    Segment AB;
    cin >> AB.p1 >> AB.p2;
    int n;
    cin >> n;
    vector<tuple<Point, bool, bool>> cps;
    for (int i = 0; i < n; i++) {
      Segment s;
      bool o, l;
      cin >> s.p1 >> s.p2 >> o >> l;
      auto ps = cross_points(AB, s);
      if (ps.size()) cps.emplace_back(ps[0], o, l);
    }
    if (!cps.size())
      cout << 0 << endl;
    else {
      sort(cps.begin(), cps.end());
      bool cur = get<1>(cps[0]) ? get<2>(cps[0]) : !get<2>(cps[0]);
      int ans = 0;
      for (int i = 1; i < cps.size(); i++)
        if (get<1>(cps[i]) ^ (cur == get<2>(cps[i]))) ans++, cur = !cur;
      cout << ans << endl;
    }
  }
  return 0;
}