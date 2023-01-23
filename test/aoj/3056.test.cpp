#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=3056"
#define ERROR "0.000001"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "src/Geometry/!geometry_temp.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geometry;
 const Real sq3= sqrt(Real(3));
 int N;
 cin >> N;
 Convex ch(N);
 for (int i= 0; i < N; i++) cin >> ch[i];
 Real ans= 1e12;
 for (int i= 0; i < N; i++) {
  Real theta= arg(ch[ch.next(i)] - ch[i]);
  auto nch= rotate(translate(ch, -ch[i]), -theta);
  Real mi= 0, ma= 0;
  for (Point p: nch) {
   Real tmp= p.y / sq3;
   mi= min(mi, p.x - tmp);
   ma= max(ma, p.x + tmp);
  }
  ans= min(ans, ma - mi);
 }
 cout << fixed << setprecision(12) << ans << endl;
 return 0;
}