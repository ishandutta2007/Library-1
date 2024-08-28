// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Prelim/0412
// competitive-verifier: TLE 0.5
#include <iostream>
#include "src/Geometry/IncrementalConvexHull.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 int N;
 cin >> N;
 IncrementalConvexHull<__int128_t> ch;
 for (int i= 0; i < N; ++i) {
  long long x, y;
  cin >> x >> y;
  ch.insert({x * 10000, y * 10000});
 }
 int Q;
 cin >> Q;
 while (Q--) {
  long long x, y;
  cin >> x >> y;
  cout << (ch.where({x * 9999, y * 9999}) == 1) << '\n';
 }
 return 0;
}