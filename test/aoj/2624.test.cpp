// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/2624
// F_2 の行列累乗
// F_2 の線形方程式・カーネル
#include <iostream>
#include "src/LinearAlgebra/LU_Decomposition.hpp"
#include "src/LinearAlgebra/Matrix.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Matrix<bool> A(N, N);
 for (int i= 0; i < N; i++)
  for (int j= 0, x; j < N; j++) cin >> x, A[i][j]= x;
 Vector<bool> v(N);
 for (int i= 0, x; i < N; i++) cin >> x, v[i]= x;
 int T;
 cin >> T;
 LU_Decomposition lu(A.pow(T));
 auto c= lu.linear_equations(v);
 if (!c.size()) cout << "none" << '\n';
 else if (!lu.kernel().empty()) cout << "ambiguous" << '\n';
 else
  for (int i= 0; i < N; i++) cout << c[i] << " \n"[i == N - 1];
 return 0;
}