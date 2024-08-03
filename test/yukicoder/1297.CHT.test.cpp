// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1297
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
 cht2.insert(0, 0);
 for (long long i= 0;;) {
  long long a, b, c= C * 2 * i;
  cin >> a >> b, a+= a, b+= b;
  cht1.insert(a - c, cht2.query(-a - c) + c * (i + 1) + b);
  if (++i == N) break;
  cht2.insert(i, cht1.query(i) + c * i);
 }
 cout << (cht1.query(N) + (N - 1) * N * C) / 2 << '\n';
 return 0;
}