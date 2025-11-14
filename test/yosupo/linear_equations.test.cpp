// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/system_of_linear_equations
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/LinearAlgebra/LU_Decomposition.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N, M;
 cin >> N >> M;
 Matrix<Mint> A(N, M);
 Vector<Mint> b(N);
 for (int i= 0; i < N; i++)
  for (int j= 0; j < M; j++) cin >> A[i][j];
 for (int i= 0; i < N; i++) cin >> b[i];
 LU_Decomposition lu(A);
 auto res= lu.linear_equations(b);
 if (!res) cout << -1 << '\n';
 else {
  auto ker= lu.kernel();
  cout << ker.size() << '\n';
  for (int j= 0; j < M; ++j) cout << res[j] << " \n"[j == M - 1];
  for (const auto &v: ker)
   for (int j= 0; j < M; ++j) cout << v[j] << " \n"[j == M - 1];
 }
 return 0;
}