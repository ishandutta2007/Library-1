// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc234/tasks/abc234_Ex
// competitive-verifier: TLE 3
// competitive-verifier: MLE 64
// https://atcoder.jp/contests/abc234/tasks/abc234_h
#include <iostream>
#include <algorithm>
#include <vector>
#include "src/DataStructure/KDTree.hpp"

//  円形クエリ
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, K;
 cin >> N >> K;
 vector<array<int, 3>> xy(N);
 for (int i= 0; i < N; ++i) cin >> xy[i][0] >> xy[i][1], xy[i][2]= i;
 KDTree<int, 2, int> kdt(xy);
 vector<pair<int, int>> ans;
 for (int i= N; i--;) {
  auto ps= kdt.enum_ball(xy[i][0], xy[i][1], K);
  for (auto p: ps)
   if (i < p) ans.emplace_back(i + 1, p + 1);
 }
 sort(ans.begin(), ans.end());
 cout << ans.size() << '\n';
 for (auto [a, b]: ans) cout << a << " " << b << '\n';
 return 0;
}