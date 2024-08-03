// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/line_add_get_min
#include <iostream>
#include "src/Optimization/ConvexHullTrick.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 ConvexHullTrick<long long, MINIMIZE> cht;
 for (int i= 0; i < N; ++i) {
  long long a, b;
  cin >> a >> b;
  cht.insert(a, b);
 }
 while (Q--) {
  int t;
  cin >> t;
  if (t) {
   int p;
   cin >> p;
   cout << cht.query(p) << '\n';
  } else {
   long long a, b;
   cin >> a >> b;
   cht.insert(a, b);
  }
 }
 return 0;
}