#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/1066"
#include <iostream>
#include <vector>
#include "src/Geometry/Line.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 using R= long double;
 using P= Point<R>;
 for (int n, m, w, h, S; cin >> n >> m >> w >> h >> S && n;) {
  S= (w * h - S) * 2;
  vector<int> l(m + 1), r(m + 1);
  vector<Line<R>> ls{line_through(P(0, 0), P(w, 0))};
  for (int i= 1; i <= m; ++i) cin >> l[i] >> r[i], ls.push_back(line_through(P(0, l[i]), P(w, r[i])));
  vector<int> cnt(m + 1);
  for (int i= n; i--;) {
   P p;
   cin >> p;
   int low= 0, high= m;
   while (high - low > 1) {
    int mid= (low + high) / 2;
    if (ls[mid].where(p) == 1) low= mid;
    else high= mid;
   }
   ++cnt[high];
  }
  for (int i= 0; i < m; ++i) cnt[i + 1]+= cnt[i];
  int ans= 0;
  for (int a= 0, b= 1; b <= m; ++b) {
   while ((l[b] - l[a] + r[b] - r[a]) * w > S) ++a;
   ans= max(ans, cnt[b] - cnt[a]);
  }
  cout << n - ans << '\n';
 }
 return 0;
}