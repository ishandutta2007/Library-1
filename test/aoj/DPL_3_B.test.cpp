// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/library/7/DPL/3/DPL_3_B
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include <algorithm>
#include <src/Misc/CartesianTree.hpp>
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int H, W;
 cin >> H >> W;
 vector<int> h(W, 0);
 int ans= 0;
 for (int i= 0; i < H; ++i) {
  for (int j= 0; j < W; ++j) {
   bool c;
   cin >> c;
   h[j]= c ? 0 : h[j] + 1;
  }
  CartesianTree ct(h);
  for (int j= 0; j < W; ++j) {
   auto [l, r]= ct.range(j);
   ans= max(ans, h[j] * (r - l));
  }
 }
 cout << ans << '\n';
 return 0;
}