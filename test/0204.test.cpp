#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Prelim/0204"
#include <iostream>
#include <vector>
#include "src/Geometry/Circle.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using namespace geo;
 using Real= long double;
 for (int R, N; cin >> R >> N && R;) {
  vector<Circle<Real>> cs(N);
  Point<Real> o(0, 0);
  int v[N];
  for (int i= 0; i < N; ++i) cin >> cs[i].o >> cs[i].r >> v[i];
  bool used[N];
  fill_n(used, N, false);
  int ans= 0;
  for (;;) {
   int tar= -1;
   Real mn= 1e10;
   for (int i= 0; i < N; ++i) {
    if (used[i]) continue;
    Real e= abs(cs[i].o), d= max(Real(0), e - v[i]);
    if (sgn(R - d) >= 0) ++ans, used[i]= true;
    else if (cs[i].o*= d / e; mn > d) mn= d, tar= i;
   }
   if (tar == -1) break;
   Point x= cs[tar].o / abs(cs[tar].o);
   Segment s(x * R, x * 2000);
   for (int i= 0; i < N; ++i)
    if (!used[i]) used[i]|= cross_points(cs[i], s).size() > 0;
  }
  cout << ans << '\n';
 }
 return 0;
}