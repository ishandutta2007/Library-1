#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/7/CGL_7_G"
#define ERROR "0.00000001"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include "src/Geometry/Circle.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 cout << fixed << setprecision(12);
 using R= long double;
 Circle<R> c, d;
 cin >> c.o >> c.r;
 cin >> d.o >> d.r;
 vector<Point<R>> ps;
 for (auto &l: common_tangent(c, d)) ps.push_back(cross_points(c, l)[0]);
 sort(ps.begin(), ps.end());
 for (auto &p: ps) cout << p.x << " " << p.y << '\n';
 return 0;
}