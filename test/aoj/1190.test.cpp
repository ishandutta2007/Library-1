#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1190"

#define ERROR "0.000001"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Geometry/!geometry_temp.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using namespace geometry;
  cout << fixed << setprecision(12);
  for (int N; cin >> N && N;) {
    vector<Circle> cs(N);
    for (int i = 0; i < N; i++) cin >> cs[i].o >> cs[i].r;
    auto height = [&](Point p) {
      Real res = 1e10;
      for (Circle c : cs)
        res = min(res, max(Real(0), c.r * c.r - norm2(c.o - p)));
      return sqrt(res);
    };
    Real ans = 0;
    for (int i = 0; i < N; i++) {
      Point p = cs[i].o;
      ans = max(ans, height(p));
      for (int j = i + 1; j < N; j++) {
        auto cp = cross_points(cs[i], cs[j]);
        if (cp.size() <= 1) continue;
        Line l = {cp[0], cp[1]};
        p = (cp[0] + cp[1]) / 2;
        ans = max(ans, height(p));
        for (int k = j + 1; k < N; k++) {
          cp = cross_points(cs[i], cs[k]);
          if (cp.size() <= 1) continue;
          Line m = {cp[0], cp[1]};
          cp = cross_points(l, m);
          if (!cp.size()) continue;
          p = cp[0];
          ans = max(ans, height(p));
        }
      }
    }
    cout << ans << endl;
  }
  return 0;
}