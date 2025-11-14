// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/1330
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// ポテンシャルUF
#include <iostream>
#include "src/DataStructure/UnionFind_Potentialized.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 for (int N, M; cin >> N >> M && N;) {
  UnionFind_Potentialized<int> uf(N);
  for (int i= 0; i < M; ++i) {
   char op;
   int a, b;
   cin >> op >> a >> b, --a, --b;
   if (op == '!') {
    int w;
    cin >> w;
    uf.unite(b, a, w);
   } else {
    if (uf.connected(a, b)) cout << uf.diff(b, a) << '\n';
    else cout << "UNKNOWN\n";
   }
  }
 }
 return 0;
}