#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1242"
#include <bits/stdc++.h>
#include "src/Geometry/!geometry_temp.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using namespace geometry;
  for (int N; cin >> N && N;) {
    Polygon g(N);
    int ymin = 2001, ymax = -2001;
    for (int i = 0; i < N; i++) {
      cin >> g[i];
      ymin = min(ymin, int(g[i].y) - 1);
      ymax = max(ymax, int(g[i].y) + 1);
    }
    int ans = 0;
    for (int y = ymin; y <= ymax; y++) {
      vector<pair<int, int>> v;
      Line l = {{0, Real(y)}, {1, Real(y)}},
           m = {{0, Real(y) + 1}, {1, Real(y) + 1}};
      for (int i = 0; i < N; i++) {
        if ((g[i].y <= y && g[g.next(i)].y <= y)
            || (g[i].y >= y + 1 && g[g.next(i)].y >= y + 1))
          continue;
        Point p = cross_points(Segment({g[i], g[g.next(i)]}), l)[0];
        Point q = cross_points(Segment({g[i], g[g.next(i)]}), m)[0];
        Real left = min(p.x, q.x), right = max(p.x, q.x);
        int li = left, ri = right;
        for (int i = -1; i <= 1; i++)
          if (0 <= sgn(left - li - i) && sgn(left - li - i - 1) < 0) {
            li = li + i;
            break;
          }
        for (int i = -1; i <= 1; i++)
          if (0 < sgn(right - ri - i + 1) && sgn(right - ri - i) <= 0) {
            ri = ri + i;
            break;
          }
        v.emplace_back(li, ri);
      }
      sort(v.begin(), v.end());
      int x = -2001;
      for (int i = 0; i < (int)v.size(); i++) {
        int l, r;
        tie(l, r) = v[i];
        if (x < l) {
          ans += r - l;
          if (i & 1) ans += l - x;
          x = r;
        } else if (x < r) {
          ans += r - x;
          x = r;
        }
      }
    }
    cout << ans << endl;
  }
  return 0;
}