#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/1226"
#include <iostream>
#include <vector>
#include <algorithm>
#include "src/Geometry/SegmentArrangement.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 cout << fixed << setprecision(6);
 using R= long double;
 using P= Point<R>;
 for (int n; cin >> n && n;) {
  vector<R> a(n + 2), b(n + 2), c(n + 2), d(n + 2);
  for (int i= 1; i <= n; ++i) cin >> a[i];
  for (int i= 1; i <= n; ++i) cin >> b[i];
  for (int i= 1; i <= n; ++i) cin >> c[i];
  for (int i= 1; i <= n; ++i) cin >> d[i];
  a[0]= b[0]= c[0]= d[0]= 0, a[n + 1]= b[n + 1]= c[n + 1]= d[n + 1]= 1;
  vector<Segment<R>> ss;
  for (int i= 0; i < n + 2; ++i) ss.emplace_back(P(a[i], 0), P(b[i], 1)), ss.emplace_back(P(0, c[i]), P(1, d[i]));
  SegmentArrangement sa(ss);
  R ans= 0;
  for (int f= sa.face_size(); f--;) ans= max(ans, sa.area(f));
  cout << ans << '\n';
 }
 return 0;
}