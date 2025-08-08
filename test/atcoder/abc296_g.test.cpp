// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc296/tasks/abc296_g
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Geometry/IncrementalConvexHull.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using namespace geo;
 int N;
 cin >> N;
 IncrementalConvexHull<int> ch;
 for (int i= 0; i < N; ++i) {
  Point<int> p;
  cin >> p;
  ch.insert(p);
 }
 int Q;
 cin >> Q;
 while (Q--) {
  Point<int> p;
  cin >> p;
  int ans= ch.where(p);
  cout << (ans > 0 ? "IN" : ans < 0 ? "OUT" : "ON") << '\n';
 }
 return 0;
}