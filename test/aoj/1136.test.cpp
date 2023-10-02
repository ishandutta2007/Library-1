#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/1136"
#include <iostream>
#include <vector>
#include <algorithm>
#include "src/Geometry/angle.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 using P= Point<int>;
 auto rot= rotate90(P(0, 0));
 for (int n; cin >> n && n;) {
  vector<vector<P>> ss0;
  {
   int m;
   cin >> m;
   vector<P> ps(m);
   for (int j= 0; j < m; ++j) cin >> ps[j];
   for (int j= m; j--;) ps[j]-= ps[0];
   for (int k= 4; k--;) {
    ss0.push_back(ps);
    for (int j= m; j--;) ps[j]= rot(ps[j]);
   }
   for (int j= 0; j < m; ++j) ps[j]-= ps[m - 1];
   reverse(ps.begin(), ps.end());
   for (int k= 4; k--;) {
    ss0.push_back(ps);
    for (int j= m; j--;) ps[j]= rot(ps[j]);
   }
  }
  for (int i= 1; i <= n; ++i) {
   int m;
   cin >> m;
   vector<P> ps(m);
   for (int j= 0; j < m; ++j) cin >> ps[j];
   for (int j= m; j--;) ps[j]-= ps[0];
   bool isok= false;
   for (const auto &qs: ss0)
    if (qs == ps) isok= true;
   if (isok) cout << i << '\n';
  }
  cout << "+++++" << '\n';
 }
 return 0;
}