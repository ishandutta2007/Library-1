// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/counting_eulerian_circuits
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
 int N, M;
 cin >> N >> M;
 Matrix<Mint> A(N, N);
 vector<int> in(N), out(N);
 int r= 0;
 for (int i= 0; i < M; ++i) {
  int u, v;
  cin >> u >> v;
  A[u][v]-= 1;
  A[v][v]+= 1;
  ++out[u];
  ++in[v];
  r= u;
 }
 vector<int> rm= {r};
 Mint ans= 1;
 for (int i= N; i--;) {
  if (in[i] != out[i]) return cout << 0 << '\n', 0;
  if (in[i]) ans*= F::fact(in[i] - 1);
  else rm.push_back(i);
 }
 ans*= LU_Decomposition(A.submatrix_rm(rm, rm)).det();
 cout << ans << '\n';
 return 0;
}