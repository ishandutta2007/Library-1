// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/matrix_det_mod_2
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <string>
#include "src/LinearAlgebra/LU_Decomposition.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Matrix<bool> A(N, N);
 for (int i= 0; i < N; ++i) {
  string a;
  cin >> a;
  for (int j= 0; j < N; ++j) A[i][j]= a[j] == '1';
 }
 cout << LU_Decomposition(A).det() << '\n';
 return 0;
}