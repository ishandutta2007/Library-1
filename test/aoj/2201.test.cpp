#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2201"
#include <iostream>
#include <vector>
#include "src/Geometry/!geometry_temp.hpp"
#include "src/Geometry/circle_functions.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geometry;
 for (int N; cin >> N && N;) {
  vector<Circle> cs(2 * N);
  Real m[N];
  for (int i= 0; i < N; i++) {
   cin >> cs[i].o >> cs[i].r >> m[i];
   cs[i + N]= {cs[i].o, cs[i].r + m[i]};
  }
  if (N == 1) {
   cout << 1 << endl;
  } else {
   vector<Line> ls;
   for (int i= 0; i < 2 * N; i++)
    for (int j= i + 1; j < 2 * N; j++) {
     auto tans= common_tangent(cs[i], cs[j]);
     for (Line l: tans) ls.push_back(l);
    }
   int ans= 0;
   for (Line l: ls) {
    int cnt= 0;
    for (int i= 0; i < N; i++) {
     Real d= dist(cs[i].o, l);
     cnt+= 0 <= sgn(d - cs[i].r) && sgn(d - cs[i].r - m[i]) <= 0;
    }
    ans= max(ans, cnt);
   }
   cout << ans << endl;
  }
 }
 return 0;
}