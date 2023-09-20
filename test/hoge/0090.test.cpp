#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Final/0090"
#include <iostream>
#include <vector>
#include "src/Geometry/Circle.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using namespace geo;
 using R= long double;
 for (int n; cin >> n && n;) {
  vector<Circle<R>> cs(n);
  for (int i= 0; i < n; ++i) {
   string s;
   cin >> s;
   cs[i].o.x= stof(s.substr(0, s.find(','))), cs[i].o.y= stof(s.substr(s.find(',') + 1)), cs[i].r= 1;
  }
  int ans= 1;
  for (int i= n; i--;)
   for (int j= i; j--;)
    for (const auto& p: cross_points(cs[i], cs[j])) {
     int cnt= 0;
     for (const auto& c: cs) cnt+= (c.where(p) != -1);
     ans= max(ans, cnt);
    }
  cout << ans << '\n';
 }
 return 0;
}