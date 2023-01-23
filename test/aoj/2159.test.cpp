#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2159"
#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include "src/Geometry/!geometry_temp.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geometry;
 int N;
 cin >> N;
 vector<Point> ps(N);
 set<pair<int, int>> S;
 for (int i= 0; i < N; i++) {
  int x, y;
  cin >> x >> y;
  ps[i]= {Real(x), Real(y)};
  S.insert({x, y});
 }
 bool isline= true;
 Line m= {ps[0], ps[1]};
 for (int i= 2; i < N; i++) isline&= m.where(ps[i]) == ON;
 if (isline) {
  cout << "No" << endl;
  return 0;
 }
 auto check= [&](Line l) {
  int cnt= 0;
  for (Point p: ps) {
   cnt+= l.where(p) == ON;
   p= l.reflect(p);
   if (sgn(p.x - round(p.x)) != 0 || sgn(p.y - round(p.y)) != 0) return false;
   if (!S.count({round(p.x), round(p.y)})) return false;
  }
  return cnt <= 2;
 };
 Convex ch= convex_hull(ps);
 int M= ch.size();
 bool isok= false;
 for (int i= 0; i < M; i++) {
  isok|= check(bisector(ch[i], ch[ch.next(i)]));
  isok|= check(Line({ch[i], ch[(i + M / 2) % M]}));
 }
 cout << (isok ? "Yes" : "No") << endl;
 return 0;
}