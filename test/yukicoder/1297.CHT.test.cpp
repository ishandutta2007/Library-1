// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1297
// competitive-verifier: TLE 1
#include <iostream>
#include <array>
#include <algorithm>
#include "src/Optimization/ConvexHullTrick.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 ConvexHullTrick<long long, MINIMIZE> cht1, cht2;
 long long N, C;
 cin >> N >> C;
 cht1.insert(0, 0);
 for (long long i= 1;; ++i) {
  long long a, b, c= C * 2 * i;
  cin >> a >> b, a+= a, b+= b;
  cht2.insert(a - c, cht1.query(-a - c) + c * (i - 1) + b);
  if (i == N) break;
  cht1.insert(i, cht2.query(i) + c * (i + 1));
 }
 cout << (cht2.query(N) + (N + 1) * N * C) / 2 << '\n';
 return 0;
}