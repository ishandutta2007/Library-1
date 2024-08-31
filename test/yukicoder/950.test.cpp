// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/950
// competitive-verifier: TLE 0.5
#include <iostream>
#include "src/Math/ModInt_Runtime.hpp"
#include "src/LinearAlgebra/Matrix.hpp"
#include "src/Math/DiscreteLogarithm.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt_Runtime<int>;
 using Mat= Matrix<Mint>;
 int p;
 cin >> p;
 Mint::set_mod(p);
 Mat A(2, 2), B(2, 2);
 for (int i= 0; i < 2; ++i)
  for (int j= 0; j < 2; ++j) cin >> A[i][j];
 for (int i= 0; i < 2; ++i)
  for (int j= 0; j < 2; ++j) cin >> B[i][j];
 auto g= [](Mat a, Mat b) { return a * b; };
 DiscreteLogarithm log1(g, g, [](Mat x) { return x[0][0].val(); }, 2ll * p + 1);
 Mint detA= A[0][0] * A[1][1] - A[0][1] * A[1][0];
 if (detA == Mint()) {
  int ans= log1(A, A, B) + 1;
  cout << (ans ? ans : -1) << '\n';
  return 0;
 }
 Mint detB= B[0][0] * B[1][1] - B[0][1] * B[1][0];
 auto f= [](Mint a, Mint b) { return a * b; };
 DiscreteLogarithm log2(f, f, [](Mint x) { return x.val(); }, p);
 int m= log2(detA, detA, 1) + 1, r= log2(detA, detA, detB) + 1;
 if (m == 0 || r == 0) {
  cout << -1 << '\n';
  return 0;
 }
 long long q= log1(A.pow(m), A.pow(r), B);
 cout << (q < 0 ? -1 : int64_t(m) * q + r) << '\n';
 return 0;
}