#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2635"
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
 vector<Point<long long>> ps(n);
 for (int i= 0; i < n; ++i) cin >> ps[i];
 bool isok= true;
 {
  IncrementalConvexHull<long long> ch;
  for (int i= 0; i < n; ++i) isok&= ch.where(ps[i]) == -1, ch.insert(ps[i]);
 }
 {
  IncrementalConvexHull<long long> ch;
  for (int i= n; i--;) isok&= ch.where(ps[i]) == -1, ch.insert(ps[i]);
 }
 cout << (isok ? "Possible" : "Impossible") << '\n';
 return 0;
}