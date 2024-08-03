// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Final/0269
#include <iostream>
#include <vector>
#include "src/Geometry/angle.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 using Real= long double;
 using P= Point<int>;
 auto contain= [&](const P &h, Real l, Real r, int a) {
  if (norm2(h) > a * a) return false;
  auto theta= angle(h);
  if (0 <= sgn(theta - l) && sgn(theta - r) <= 0) return true;
  if (0 <= sgn(theta - l - M_PI * 2) && sgn(theta - r - M_PI * 2) <= 0) return true;
  if (0 <= sgn(theta - l + M_PI * 2) && sgn(theta - r + M_PI * 2) <= 0) return true;
  return false;
 };
 for (int H, R; cin >> H >> R && H;) {
  vector<P> h(H);
  for (int i= 0; i < H; ++i) cin >> h[i];
  int U, M, S;
  Real du, dm, ds;
  cin >> U >> M >> S >> du >> dm >> ds;
  du= degree_to_radian(du), dm= degree_to_radian(dm), ds= degree_to_radian(ds);
  vector<P> u(U), m(M), s(S);
  for (int i= 0; i < U; ++i) cin >> u[i];
  for (int i= 0; i < M; ++i) cin >> m[i];
  for (int i= 0; i < S; ++i) cin >> s[i];
  Real w[R];
  int a[R];
  for (int i= 0; i < R; ++i) cin >> w[i] >> a[i], w[i]= degree_to_radian(w[i]);
  vector<int> ans;
  int mx= 0;
  for (int i= 0; i < H; ++i) {
   int cnt= 0;
   for (int j= R; j--;) {
    if (!contain(h[i], w[j] - du / 2, w[j] + du / 2, a[j])) continue;
    bool isok= true;
    for (int k= U; k--;) isok&= !contain(h[i] - u[k], w[j] - du / 2, w[j] + du / 2, a[j]);
    for (int k= M; k--;) isok&= !contain(h[i] - m[k], w[j] - dm / 2, w[j] + dm / 2, a[j]);
    for (int k= S; k--;) isok&= !contain(h[i] - s[k], w[j] - ds / 2, w[j] + ds / 2, a[j]);
    cnt+= isok;
   }
   if (mx == cnt) ans.push_back(i + 1);
   else if (mx < cnt) mx= cnt, ans= {i + 1};
  }
  if (mx)
   for (int i= 0, e= ans.size(); i < e; ++i) cout << ans[i] << " \n"[i == e - 1];
  else cout << "NA" << '\n';
 }
 return 0;
}