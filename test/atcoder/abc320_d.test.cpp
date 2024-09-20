// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc320/tasks/abc320_d
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// ポテンシャルUF
#include <iostream>
#include "src/DataStructure/UnionFind_Potentialized.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, M;
 cin >> N >> M;
 UnionFind_Potentialized<long long> ufx(N), ufy(N);
 for (int i= 0; i < M; ++i) {
  int A, B, X, Y;
  cin >> A >> B >> X >> Y, --A, --B;
  ufx.unite(B, A, X), ufy.unite(B, A, Y);
 }
 for (int i= 0; i < N; ++i) {
  if (ufx.connected(0, i)) cout << ufx.diff(i, 0) << " " << ufy.diff(i, 0) << '\n';
  else cout << "undecidable" << '\n';
 }
 return 0;
}