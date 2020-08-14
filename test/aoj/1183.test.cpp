#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1183"

#define ERROR "0.001"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Geometry/_geometry_temp.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using namespace geometry;
  cout << fixed << setprecision(12);
  for (int N; cin >> N && N;) {
    vector<Circle> cs(N);
    for (int i = 0; i < N; i++) cin >> cs[i].o >> cs[i].r;
    vector<Point> ps(2 * N);
    vector<Segment> ss(N - 1);
    ps[0] = cs[0].o;
    ps[2 * N - 1] = cs[N - 1].o;
    for (int i = 0; i < N - 1; i++) {
      auto pp = cross_points(cs[i], cs[i + 1]);
      ps[2 * i + 1] = pp[0];
      ps[2 * i + 2] = pp[1];
      ss[i] = {pp[0], pp[1]};
    }
    Real dp[2 * N];
    fill(dp, dp + 2 * N, 1e9);
    dp[0] = 0;
    for (int i = 0; i < 2 * N - 1; i++)
      for (int j = i & 1 ? i + 2 : i + 1; j < 2 * N; j++) {
        bool isok = true;
        Segment s = {ps[i], ps[j]};
        for (int k = (i + 1) / 2; k < (j - 1) / 2; k++)
          isok &= cross_points(ss[k], s).size() > 0;
        if (isok) dp[j] = min(dp[j], dp[i] + dist(ps[i], ps[j]));
      }
    cout << dp[2 * N - 1] << endl;
  }
  return 0;
}