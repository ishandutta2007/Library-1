// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/segment_add_get_min
#include <iostream>
#include "src/Optimization/LiChaoTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 LiChaoTree lct([](long long x, int a, long long b) { return a * x + b; });
 auto cht= lct.make_tree<MINIMIZE>();
 while (N--) {
  long long l, r, a, b;
  cin >> l >> r >> a >> b;
  cht.insert(l, r, a, b);
 }
 while (Q--) {
  bool op;
  cin >> op;
  if (op) {
   long long p;
   cin >> p;
   auto [ans, id]= cht.query(p);
   if (id == -1) cout << "INFINITY" << '\n';
   else cout << ans << '\n';
  } else {
   long long l, r, a, b;
   cin >> l >> r >> a >> b;
   cht.insert(a, b, l, r);
  }
 }
 return 0;
}