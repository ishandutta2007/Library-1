#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/1183"
#define ERROR "0.0001"
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include "src/Geometry/Circle.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 cout << fixed << setprecision(6);
 using R= long double;
 for (int N; cin >> N && N;) {
  vector<Circle<R>> cs(N);
  for (int i= 0; i < N; i++) cin >> cs[i].o >> cs[i].r;
  vector<Segment<R>> ss;
  ss.emplace_back(cs[0].o, cs[0].o);
  for (int i= 1; i < N; ++i) {
   auto pp= cross_points(cs[i - 1], cs[i]);
   ss.emplace_back(pp[0], pp[1]);
  }
  ss.emplace_back(cs[N - 1].o, cs[N - 1].o);
  R dp[N + 1][2];
  for (int i= 1; i <= N; ++i) dp[i][0]= dp[i][1]= 1e10;
  dp[0][0]= dp[0][1]= 0;
  for (int i= 1; i <= N; ++i) {
   for (int j= i; j--;) {
    for (int a= 2; a--;)
     for (int b= 2; b--;) {
      Segment s= {ss[j][a], ss[i][b]};
      bool isok= true;
      for (int k= j + 1; k < i; ++k) isok&= cross_points(ss[k], s).size() > 0;
      if (isok) dp[i][b]= min(dp[i][b], dp[j][a] + s.length());
     }
   }
  }
  cout << dp[N][0] << '\n';
 }
}