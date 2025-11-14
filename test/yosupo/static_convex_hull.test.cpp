// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/static_convex_hull
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
  Convex ch(ps);
  cout << ch.size() << '\n';
  for (const auto &p: ch) cout << p.x << " " << p.y << '\n';
 }
 return 0;
}