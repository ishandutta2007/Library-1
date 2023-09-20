#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Final/0129"
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
  for (int i= 0; i < n; ++i) cin >> cs[i].o >> cs[i].r;
  int m;
  cin >> m;
  while (m--) {
   Segment<R> s;
   cin >> s.p >> s.q;
   bool safe= false;
   for (const auto &c: cs) safe|= cross_points(s, c).size();
   cout << (safe ? "Safe" : "Danger") << '\n';
  }
 }
 return 0;
}