// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1907
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/LinearAlgebra/characteristic_polynomial.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N;
 cin >> N;
 Matrix<Mint> M0(N, N), M1(N, N);
 for (int i= 0; i < N; ++i)
  for (int j= 0; j < N; ++j) cin >> M0[i][j];
 for (int i= 0; i < N; ++i)
  for (int j= 0; j < N; ++j) cin >> M1[i][j];
 auto poly= det_of_first_degree_poly_mat(M0, M1);
 for (int i= 0; i <= N; ++i) cout << poly[i] << '\n';
 return 0;
}