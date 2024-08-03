// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/1342
// competitive-verifier: ERROR 0.00001
#include <iostream>
#include <iomanip>
#include <vector>
#include "src/Geometry/Circle.hpp"
#include "src/Geometry/Polygon.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 cout << fixed << setprecision(12);
 using R= long double;
 for (int n, w; cin >> n >> w && n;) {
  vector<Point<R>> ps(n);
  vector<R> h(n);
  for (int i= 0; i < n; ++i) cin >> ps[i] >> h[i];
  auto rad= [&](R r, R hei) { return r <= hei ? r : sqrt(hei * (2 * r - hei)); };
  auto check= [&](R r) {
   R ma= rad(r, w);
   if (ma >= 50) return false;
   Polygon<R> g({{ma, ma}, {100 - ma, ma}, {100 - ma, 100 - ma}, {ma, 100 - ma}});
   vector<Circle<R>> cs(n);
   for (int i= 0; i < n; i++) cs[i]= {ps[i], rad(r, h[i])};
   vector<Point<R>> cps;
   for (auto &p: g) cps.push_back(p);
   for (auto &e: g.edges())
    for (auto &c: cs)
     for (auto &p: cross_points(e, c)) cps.push_back(p);
   for (int i= n; i--;)
    for (int j= i; j--;)
     for (auto &p: cross_points(cs[i], cs[j])) cps.push_back(p);
   for (auto &p: cps) {
    bool isok= g.where(p) != -1;
    for (auto &c: cs) isok&= c.where(p) != 1;
    if (isok) return true;
   }
   return false;
  };
  R low= 0, high= 1000;
  for (int cnt= 100; cnt--;) {
   if (R mid= (low + high) / 2; check(mid)) low= mid;
   else high= mid;
  }
  cout << low << '\n';
 }
 return 0;
}