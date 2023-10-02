#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2448"
#define ERROR "0.00000001"
#include <iostream>
#include <iomanip>
#include <vector>
#include "src/Geometry/SegmentArrangement.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 cout << fixed << setprecision(10);
 using R= long double;
 int N;
 cin >> N;
 vector<Point<R>> ps(N);
 for (int i= 0; i < N; ++i) cin >> ps[i];
 vector<Segment<R>> ss;
 for (int i= N; --i;) ss.emplace_back(ps[i - 1], ps[i]);
 SegmentArrangement sa(ss);
 R ans= 0;
 for (int f= sa.face_size(); f--;)
  if (R a= sa.area(f); a > 0) ans+= a;
 cout << ans << '\n';
 return 0;
}