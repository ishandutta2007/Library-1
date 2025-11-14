// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/matrix_det_arbitrary_mod
// competitive-verifier: TLE 2
// competitive-verifier: MLE 64
#include <iostream>
#include "src/Math/ModInt_Runtime.hpp"
#include "src/LinearAlgebra/characteristic_polynomial.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt_Runtime<int>;
 int N, m;
 cin >> N >> m;
 Mint::set_mod(m);
 Matrix<Mint> a(N, N);
 for (int i= 0; i < N; ++i)
  for (int j= 0; j < N; ++j) cin >> a[i][j];
 cout << characteristic_polynomial(a * -1, true)[0] << '\n';
 return 0;
}
