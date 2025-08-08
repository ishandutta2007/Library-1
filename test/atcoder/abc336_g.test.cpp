// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc336/tasks/abc336_g
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
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
 Matrix<Mint> A(8, 8);
 vector<int> in(8), out(8);
 Mint ans= 1;
 int r= 0;
 int N= 0;
 for (int s= 0; s < 16; ++s) {
  int u= s >> 1, v= s & 7, X;
  cin >> X;
  N+= X;
  A[u][v]-= X;
  A[v][v]+= X;
  out[u]+= X;
  in[v]+= X;
  ans*= F::finv(X);
  if (X) r= u;
 }
 int s= -1, t= -1;
 for (int i= 8; i--;) {
  int tmp= in[i] - out[i];
  if (abs(tmp) > 1) {
   s= -2;
  } else if (tmp == 1) {
   if (s == -1) s= i;
   else s= -2;
  } else if (tmp == -1) {
   if (t == -1) t= i;
   else s= -2;
  }
 }
 if (s == -2) return cout << 0 << '\n', 0;
 if (s != -1) {
  A[s][t]-= 1;
  A[t][t]+= 1;
  ++out[s];
  ++in[t];
 } else {
  ans*= N;
 }
 vector<int> rm= {r};
 for (int i= 8; i--;)
  if (out[i]) ans*= F::fact(out[i] - 1);
  else rm.push_back(i);
 ans*= LU_Decomposition(A.submatrix_rm(rm, rm)).det();
 cout << ans << '\n';
 return 0;
}