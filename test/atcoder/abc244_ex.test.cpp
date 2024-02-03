#define PROBLEM "https://atcoder.jp/contests/abc244/tasks/abc244_Ex"
// https://atcoder.jp/contests/abc244/tasks/abc244_h
#include <iostream>
#include "src/Optimization/ConvexHullTrick.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int Q;
 cin >> Q;
 ConvexHullTrick_XY<int64_t> cht;
 while (Q--) {
  int X, Y, A, B;
  cin >> X >> Y >> A >> B;
  cht.insert(X, Y);
  cout << cht.get_max(A, B) << '\n';
 }
 return 0;
}