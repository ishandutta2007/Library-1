#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/5/CGL_5_A"
#define ERROR "0.00000001"
#include <iostream>
#include <iomanip>
#include <vector>
#include "src/Geometry/closest_pair.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 int n;
 cin >> n;
 vector<Point<long double>> ps(n);
 for (int i= 0; i < n; i++) cin >> ps[i];
 auto [p, q]= closest_pair(ps);
 cout << fixed << setprecision(12) << dist(p, q) << '\n';
 return 0;
}