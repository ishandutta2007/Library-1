// competitive-verifier: PROBLEM https://atcoder.jp/contests/GoodByeDay2/tasks/agc051_d
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// https://atcoder.jp/contests/agc051/tasks/agc051_d
#include <iostream>
#include <vector>
#include "src/LinearAlgebra/LU_Decomposition.hpp"
#include "src/Math/ModInt.hpp"
#include "src/Math/FactorialPrecalculation.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 using F= FactorialPrecalculation<Mint>;
 int a[4];
 cin >> a[0] >> a[1] >> a[2] >> a[3];
 for (int i= 4; i--;)
  if ((a[i] ^ a[0]) & 1) return cout << 0 << '\n', 0;
 int mn= min({a[0], a[1], a[2], a[3]});
 Mint ans= 0;
 for (int x= 0; x <= mn; ++x) {
  int k= mn - x - x;
  Matrix<Mint> A(4, 4);
  Mint s= 1;
  for (int i= 4; i--;) {
   int j= (i + 1) % 4;
   int z= (a[i] - k) / 2;
   A[i][j]-= z;
   A[j][j]+= z;
   A[j][i]-= a[i] - z;
   A[i][i]+= a[i] - z;
   s*= F::finv(z);
   s*= F::finv(a[i] - z);
  }
  s*= LU_Decomposition(A.submatrix_rm({0}, {0})).det();
  for (int i= 4; i--;) s*= F::fact(A[i][i].val() - 1);
  s*= A[0][0];
  ans+= s;
 }
 cout << ans << '\n';
 return 0;
}