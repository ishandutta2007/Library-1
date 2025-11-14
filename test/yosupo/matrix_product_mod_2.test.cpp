// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/matrix_product_mod_2
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
#include <iostream>
#include <string>
#include "src/LinearAlgebra/LU_Decomposition.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M, K;
 cin >> N >> M >> K;
 Matrix<bool> A(N, M), B(M, K);
 for (int i= 0; i < N; ++i) {
  string a;
  cin >> a;
  for (int j= 0; j < M; ++j) A[i][j]= a[j] - '0';
 }
 for (int i= 0; i < M; ++i) {
  string b;
  cin >> b;
  for (int j= 0; j < K; ++j) B[i][j]= b[j] - '0';
 }
 auto C= A * B;
 for (int i= 0; i < N; ++i) {
  for (int j= 0; j < K; ++j) cout << C[i][j];
  cout << '\n';
 }
 return 0;
}