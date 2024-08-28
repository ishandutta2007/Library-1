// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Prelim/0323
// competitive-verifier: ERROR 0.00001
// competitive-verifier: TLE 0.5
#include <iostream>
#include <iomanip>
#include "src/Optimization/golden_search.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using R= long double;
 cout << fixed << setprecision(10);
 int N;
 cin >> N;
 int x[N], r[N];
 for (int i= 0; i < N; ++i) cin >> x[i] >> r[i];
 auto f= [&](R t) {
  R ret= 1e14;
  for (int i= 0; i < N; ++i) ret= min(ret, R(r[i]) * r[i] - R(t - x[i]) * (t - x[i]));
  return sqrt(ret);
 };
 int left= -1e7, right= 1e7;
 for (int i= 0; i < N; ++i) left= max(left, x[i] - r[i]), right= min(right, x[i] + r[i]);
 cout << golden_search<MAXIMIZE>(f, left, right).second << '\n';
 return 0;
}