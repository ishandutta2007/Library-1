// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/2635
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Geometry/IncrementalConvexHull.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 int n;
 cin >> n;
 vector<Point<int>> ps(n);
 for (int i= 0; i < n; ++i) cin >> ps[i];
 bool isok= true;
 {
  IncrementalConvexHull<int> ch;
  for (int i= 0; i < n; ++i) isok&= ch.where(ps[i]) == -1, ch.insert(ps[i]);
 }
 {
  IncrementalConvexHull<int> ch;
  for (int i= n; i--;) isok&= ch.where(ps[i]) == -1, ch.insert(ps[i]);
 }
 cout << (isok ? "Possible" : "Impossible") << '\n';
 return 0;
}