// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/1132
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Geometry/Circle.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using namespace geo;
 using R= long double;
 int N;
 while (cin >> N && N) {
  vector<Point<R>> ps(N);
  for (int i= 0; i < N; ++i) cin >> ps[i];
  int ans= 1;
  for (int i= N; i--;)
   for (int j= i; j--;)
    for (const auto &o: cross_points(Circle<R>(ps[i], 1), Circle<R>(ps[j], 1))) {
     Circle<R> c(o, 1);
     int cnt= 0;
     for (const auto &p: ps) cnt+= c.where(p) != -1;
     ans= max(ans, cnt);
    }
  cout << ans << '\n';
 }
 return 0;
}