// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/matrix_product
// competitive-verifier: TLE 0.5
#include <iostream>
#include "src/LinearAlgebra/Matrix.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N, M, K;
 cin >> N >> M >> K;
 Matrix<Mint> A(N, M), B(M, K);
 for (int i= 0; i < N; ++i)
  for (int j= 0; j < M; ++j) cin >> A[i][j];
 for (int i= 0; i < M; ++i)
  for (int j= 0; j < K; ++j) cin >> B[i][j];
 auto C= A * B;
 for (int i= 0; i < N; ++i)
  for (int j= 0; j < K; ++j) cout << C[i][j] << " \n"[j == K - 1];
 return 0;
}