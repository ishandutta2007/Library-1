// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/furthest_pair
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Geometry/Convex.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using namespace geo;
 int T;
 cin >> T;
 while (T--) {
  int N;
  cin >> N;
  vector<Point<int>> ps(N);
  for (int i= 0; i < N; ++i) cin >> ps[i];
  auto [p0, p1]= farthest_pair(ps);
  int ans0= -1, ans1= -1;
  for (int i= N; i--;) {
   if (ps[i] == p0 && ans0 == -1) ans0= i;
   else if (ps[i] == p1 && ans1 == -1) ans1= i;
  }
  cout << ans0 << ' ' << ans1 << '\n';
 }
 return 0;
}