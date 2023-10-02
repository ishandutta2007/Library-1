#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/1047"
#define ERROR "0.00000001"
#include <iostream>
#include <iomanip>
#include <vector>
#include "src/Misc/compress.hpp"
#include "src/Geometry/Circle.hpp"
#include "src/Geometry/angle.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 cout << fixed << setprecision(10);
 using R= long double;
 for (int n; cin >> n && n;) {
  vector<Circle<R>> cs(n);
  for (int i= 0; i < n; ++i) cin >> cs[i].o >> cs[i].r;
  R ans= 0;
  for (int i= n; i--;) {
   bool isok= true;
   vector<R> a{-M_PI, M_PI};
   vector<pair<R, R>> query;
   for (int j= n; j--;) {
    if (i == j) continue;
    if (cs[j].in(cs[i])) {
     isok= false;
     break;
    }
    auto cp= cross_points(cs[i], cs[j]);
    if (cp.size() == 2) {
     auto l= normalize_radian(angle(cp[0] - cs[i].o));
     auto r= normalize_radian(angle(cp[1] - cs[i].o));
     query.emplace_back(l, r);
     a.emplace_back(l), a.emplace_back(r);
    }
   }
   if (!isok) continue;
   auto id= compress(a);
   int m= a.size() - 1;
   vector<int> sum(m + 1);
   for (auto [l, r]: query) {
    ++sum[id(l)], --sum[id(r)];
    if (l > r) ++sum[0], --sum[m];
   }
   for (int j= 0; j < m; ++j) sum[j + 1]+= sum[j];
   for (int j= 0; j < m; ++j)
    if (sum[j] == 0) ans+= (a[j + 1] - a[j]) * cs[i].r;
  }
  cout << ans << '\n';
 }
 return 0;
}