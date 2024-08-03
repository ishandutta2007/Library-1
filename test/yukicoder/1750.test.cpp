// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1750
// 行列累乗
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/LinearAlgebra/Matrix.hpp"
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
 cout << mat.pow(T)[0][0] << '\n';
 return 0;
}