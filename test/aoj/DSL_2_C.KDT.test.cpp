// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/all/DSL_2_C
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// 列挙のverify

#include <iostream>
#include <vector>
#include <algorithm>
#include "src/DataStructure/KDTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int n;
 cin >> n;
 vector<array<int, 3>> xy(n);
 for (int i= 0; i < n; ++i) cin >> xy[i][0] >> xy[i][1], xy[i][2]= i;
 KDTree<int, 2, int> kdt(xy);
 int q;
 cin >> q;
 while (q--) {
  int sx, tx, sy, ty;
  cin >> sx >> tx >> sy >> ty;
  auto ans= kdt.enum_cuboid(sx, tx, sy, ty);
  sort(ans.begin(), ans.end());
  for (auto x: ans) cout << x << '\n';
  cout << '\n';
 }
 return 0;
}