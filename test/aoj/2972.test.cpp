// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/2972
// competitive-verifier: ERROR 0.0001
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include "src/Optimization/golden_search.hpp"
#include "src/Geometry/Point.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 using R= long double;
 int N, K;
 cin >> N >> K;
 vector<Point<R>> ps(N);
 for (int i= 0; i < N; ++i) cin >> ps[i];
 auto score= [&](const Point<R> &p) {
  vector<R> dis(N);
  for (int i= 0; i < N; ++i) dis[i]= dist(ps[i], p);
  sort(dis.rbegin(), dis.rend());
  R ret= 0;
  for (int i= 0; i < K; ++i) ret+= dis[i];
  return ret;
 };
 auto f= [&](R x) {
  auto g= [&](R y) { return score({x, y}); };
  return golden_search<MINIMIZE>(g, -1000.0, 1000.0).second;
 };
 cout << fixed << setprecision(10) << golden_search<MINIMIZE>(f, -1000.0, 1000.0).second << '\n';
 return 0;
}