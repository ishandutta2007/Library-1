// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/counting_spanning_tree_directed
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/LinearAlgebra/LU_Decomposition.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N, M, r;
 cin >> N >> M >> r;
 Matrix<Mint> A(N, N);
 for (int i= 0; i < M; ++i) {
  int u, v;
  cin >> u >> v;
  A[v][v]+= 1;
  A[u][v]-= 1;
 }
 cout << LU_Decomposition(A.submatrix_rm({r}, {r})).det() << '\n';
 return 0;
}