// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/JAG/Summer/2235
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/DataStructure/OnlineDynamicConnectivity.hpp"
using namespace std;
int main() {
 std::cin.tie(nullptr);
 std::ios::sync_with_stdio(false);
 int n, k;
 cin >> n >> k;
 OnlineDynamicConnectivity dicon(n);
 while (k--) {
  int op, u, v;
  cin >> op >> u >> v;
  if (op == 1) dicon.link(u, v);
  if (op == 2) dicon.cut(u, v);
  if (op == 3) cout << (dicon.connected(u, v) ? "YES" : "NO") << '\n';
 }
}