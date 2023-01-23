#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2256"
#define ERROR "0.000000001"
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include "src/Geometry/!geometry_temp.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geometry;
 cout << fixed << setprecision(12);
 for (int W, H, N; cin >> W >> H >> N && N;) {
  vector<Point> ps= {{0, 0}, {0, Real(H)}, {Real(W), 0}, {Real(W), Real(H)}};
  ps.resize(2 * N + 4);
  for (int i= 0; i < 2 * N; i++) cin >> ps[i + 4];
  N+= 2;
  Segment left= {ps[0], ps[1]}, right= {ps[2], ps[3]};
  auto check= [&](Line l) {
   int on_cnt= 0, sum= 0;
   for (int k= 4; k < 2 * N; k++) {
    sum+= l.where(ps[k]);
    if (l.where(ps[k]) == 0) on_cnt++;
   }
   sum= abs(sum);
   while (on_cnt && sum) on_cnt--, sum--;
   return sum == 0;
  };
  vector<Real> event;
  for (int i= 0; i < 2 * N; i++)
   for (int j= i + 1; j < 2 * N; j++) {
    Line l= {ps[i], ps[j]};
    if (check(l)) {
     auto cp= cross_points(left, l);
     if (!cp.size()) continue;
     if (!cross_points(right, l).size()) continue;
     event.emplace_back(cp[0].y);
    }
   }
  sort(event.begin(), event.end());
  event.erase(unique(event.begin(), event.end()), event.end());
  auto f= [&](Real x) {
   Real mi= H + 10, ma= -1;
   for (int i= 0; i < 2 * N; i++) {
    if (sgn(ps[i].x) == 0) continue;
    Line l= {{0, x}, ps[i]};
    if (check(l)) {
     auto cp= cross_points(right, l);
     if (!cp.size()) continue;
     mi= min(mi, cp[0].y);
     ma= max(ma, cp[0].y);
    }
   }
   if (ma < 0) return Real(0);
   return ma - mi;
  };
  Real ans= 0;
  for (int i= 0; i + 1 < (int)event.size(); i++) ans+= (event[i + 1] - event[i]) * f((event[i] + event[i + 1]) / 2);
  ans/= H * H;
  cout << ans << endl;
 }
 return 0;
}