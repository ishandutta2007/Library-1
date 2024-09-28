// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/matrix_rank_mod_2
// competitive-verifier: TLE 1
// competitive-verifier: MLE 512
#include <iostream>
#include <string>
#include "src/LinearAlgebra/LU_Decomposition.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 if (N <= M) {
  Matrix<bool> A(N, M);
  for (int i= 0; i < N; ++i) {
   string a;
   cin >> a;
   for (int j= 0; j < M; ++j) A[i][j]= a[j] - '0';
  }
  cout << LU_Decomposition(A).rank() << '\n';
 } else {
  Matrix<bool> A(M, N);
  for (int i= 0; i < N; ++i) {
   string a;
   cin >> a;
   for (int j= 0; j < M; ++j) A[j][i]= a[j] - '0';
  }
  cout << LU_Decomposition(A).rank() << '\n';
 }
 return 0;
}