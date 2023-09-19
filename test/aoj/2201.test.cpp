#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2201"
#include <iostream>
#include <vector>
#include "src/Geometry/Circle.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 using R= long double;
 for (int N; cin >> N && N;) {
  vector<Circle<R>> cs(2 * N);
  for (int i= 0; i < N; ++i) {
   Point<R> o;
   R r, m;
   cin >> o >> r >> m;
   cs[i]= {o, r}, cs[i + N]= {o, r + m};
  }
  vector<Line<R>> ls;
  for (int i= 2 * N; i--;)
   for (int j= i; j--;)
    for (const auto &l: common_tangent(cs[i], cs[j])) ls.push_back(l);
  int ans= 1;
  for (const auto &l: ls) {
   int cnt= 0;
   for (int i= N; i--;) cnt+= cs[i].where(l) != 1 && cs[i + N].where(l) != -1;
   ans= max(ans, cnt);
  }
  cout << ans << '\n';
 }
 return 0;
}