// competitive-verifier: PROBLEM https://atcoder.jp/contests/arc090/tasks/arc090_d
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// https://atcoder.jp/contests/arc090/tasks/arc090_b
#include <iostream>
#include "src/DataStructure/UnionFind_Potentialized.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 UnionFind_Potentialized<long long> uf(N);
 bool isok= true;
 for (int i= 0; i < M; ++i) {
  int L, R, D;
  cin >> L >> R >> D, --L, --R;
  isok&= uf.unite(L, R, D);
 }
 cout << (isok ? "Yes" : "No") << '\n';
 return 0;
}