#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=3049"

#define ERROR "0.00000001"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Geometry/!geometry_temp.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using namespace geometry;
  int N;
  cin >> N;
  Convex ch(N);
  for (int i = 0; i < N; i++) cin >> ch[i];
  int M;
  cin >> M;
  vector<Point> ps(M);
  for (int i = 0; i < M; i++) cin >> ps[i];
  Real ans = 0;
  for (int i = 0; i < M; i++) {
    Convex dh = ch;
    for (int j = 0; j < M; j++)
      if (j != i) dh = dh.cut(bisector(ps[i], ps[j]));
    for (Point p : dh) ans = max(ans, dist(p, ps[i]));
  }
  cout << fixed << setprecision(12) << ans << endl;
  return 0;
}