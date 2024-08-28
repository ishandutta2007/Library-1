// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/1107
// competitive-verifier: TLE 0.5
#include <iostream>
#include <iomanip>
#include <vector>
#include "src/Geometry/angle.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 cout << fixed << setprecision(1);
 using R= long double;
 for (int N; cin >> N && N;) {
  vector<Point<R>> ps(N);
  for (int i= 0; i < N; ++i) cin >> ps[i];
  R ans= 0;
  vector<int> used(N);
  Point<R> cur(0, 0), d(0, 1);
  for (int i= N; i--;) {
   R mn= M_PI, dis= 1e10;
   int nx= -1;
   for (int j= N; j--;) {
    if (used[j]) continue;
    R t= angle(ps[j] - cur, d), a= dist2(cur, ps[j]);
    if (mn > t) mn= t, nx= j, dis= a;
    else if (!sgn(mn - t))
     if (dis > a) dis= a, nx= j;
   }
   d= ps[nx] - cur, cur= ps[nx], used[nx]= true, ans+= sqrt(dis);
  }
  cout << ans << '\n';
 }
 return 0;
}