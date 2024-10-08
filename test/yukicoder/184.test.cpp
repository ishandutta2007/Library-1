// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/184
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// 行列(F_2)のrankのverify
#include <iostream>
#include "src/LinearAlgebra/LU_Decomposition.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Matrix<bool> A(N, 61);
 for (int i= 0; i < N; i++) {
  long long a;
  cin >> a;
  for (int j= 0; j <= 60; j++) A[i][j]= (a >> j) & 1;
 }
 cout << (1ll << LU_Decomposition(A).rank()) << '\n';
 return 0;
}