// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1750
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// 最小多項式を用いた行列累乗
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/LinearAlgebra/Matrix.hpp"
#include "src/LinearAlgebra/MinimalPolynomial.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 long long N, M, T;
 cin >> N >> M >> T;
 Matrix<Mint> mat(N, N);
 while (M--) {
  int s, t;
  cin >> s >> t;
  mat[s][t]= mat[t][s]= 1;
 }
 Vector<Mint> vec(N);
 vec[0]= 1;
 cout << MinimalPolynomial(mat, vec).pow(T)[0] << '\n';
 return 0;
}