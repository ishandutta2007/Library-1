// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc323/tasks/abc323_g
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <numeric>
#include "src/Math/ModInt.hpp"
#include "src/LinearAlgebra/characteristic_polynomial.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N;
 cin >> N;
 vector<int> P(N);
 for (int i= 0; i < N; ++i) cin >> P[i];
 Matrix<Mint> M0(N, N), M1(N, N);
 for (int i= N; i--;)
  for (int j= i; j--;) {
   if (P[i] > P[j]) M0[i][j]= M0[j][i]= -1, M0[i][i]+= 1, M0[j][j]+= 1;
   else M1[i][j]= M1[j][i]= -1, M1[i][i]+= 1, M1[j][j]+= 1;
  }
 auto p= det_of_first_degree_poly_mat(M0.submatrix_rm({0}, {0}), M1.submatrix_rm({0}, {0}));
 for (int k= 0; k < N; ++k) cout << p[k] << " \n"[k + 1 == N];
 return 0;
}