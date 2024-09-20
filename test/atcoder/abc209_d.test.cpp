// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc209/tasks/abc209_d
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// 偶奇
#include <iostream>
#include "src/DataStructure/UnionFind_Potentialized.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, Q;
 cin >> N >> Q;
 UnionFind_Potentialized<bool> uf(N);
 for (int i= N - 1; i--;) {
  int a, b;
  cin >> a >> b;
  uf.unite(--a, --b, 1);
 }
 while (Q--) {
  int c, d;
  cin >> c >> d;
  cout << (uf.diff(--c, --d) ? "Road" : "Town") << '\n';
 }
 return 0;
}