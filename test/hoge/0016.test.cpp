#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Final/0016"
#include <iostream>
#include "src/Geometry/Point.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using namespace geo;
 Point<long double> cur{0, 0}, dir{0, 1};
 for (string in; cin >> in && in != "0,0";) {
  int d= stoi(in.substr(0, in.find(','))), a= stoi(in.substr(in.find(',') + 1));
  cur+= dir * d;
  dir= rotate<long double>(degree_to_radian(-a))(dir);
 }
 long double x, y;
 modfl(cur.x, &x), modfl(cur.y, &y);
 cout << x << '\n' << y << '\n';
 return 0;
}