// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc280/tasks/abc280_f
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

// ポテンシャルUF
#include <iostream>
#include <vector>
#include "src/DataStructure/UnionFind_Potentialized.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M, Q;
 cin >> N >> M >> Q;
 UnionFind_Potentialized<long long> uf(N);
 vector<bool> isinf(N, false);
 for (int i= 0; i < M; i++) {
  int A, B;
  cin >> A >> B, A--, B--;
  long long C;
  cin >> C;
  if (!uf.unite(A, B, C)) isinf[A]= true, isinf[B]= true;
 }
 for (int i= 0; i < N; i++)
  if (isinf[i]) isinf[uf.leader(i)]= true;
 while (Q--) {
  int X, Y;
  cin >> X >> Y, X--, Y--;
  if (!uf.connected(X, Y)) cout << "nan" << '\n';
  else if (isinf[uf.leader(X)]) cout << "inf" << '\n';
  else cout << uf.diff(X, Y) << '\n';
 }
 return 0;
}