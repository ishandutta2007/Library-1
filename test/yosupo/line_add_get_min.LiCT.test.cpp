#define PROBLEM "https://judge.yosupo.jp/problem/line_add_get_min"
#include <iostream>
#include "src/Optimization/LiChaoTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 LiChaoTree lct([](int x, int a, long long b) { return a * x + b; });
 auto cht= lct.make_tree<MINIMIZE>();
 while (N--) {
  long long a, b;
  cin >> a >> b;
  cht.insert(a, b);
 }
 while (Q--) {
  bool op;
  cin >> op;
  if (op) {
   long long p;
   cin >> p;
   cout << cht.query(p).first << '\n';
  } else {
   long long a, b;
   cin >> a >> b;
   cht.insert(a, b);
  }
 }
 return 0;
}