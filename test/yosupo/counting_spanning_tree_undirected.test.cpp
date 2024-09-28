// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/counting_spanning_tree_undirected
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
 int N, M;
 cin >> N >> M;
 Matrix<Mint> A(N, N);
 for (int i= 0; i < M; ++i) {
  int u, v;
  cin >> u >> v;
  A[u][u]+= 1;
  A[v][v]+= 1;
  A[u][v]-= 1;
  A[v][u]-= 1;
 }
 cout << LU_Decomposition(A.submatrix_rm({0}, {0})).det() << '\n';
 return 0;
}