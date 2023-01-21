#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2624"
#include <iostream>
#include <vector>
#include "src/LinearAlgebra/LUDecomposition.hpp"
#include "src/LinearAlgebra/Matrix.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mat= SquareMatrix<bool, 300>;
 int N;
 cin >> N;
 Mat A;
 for (int i= 0; i < N; i++)
  for (int j= 0, x; j < N; j++) cin >> x, A[i][j]= x;
 vector<bool> v(N);
 for (int i= 0, x; i < N; i++) cin >> x, v[i]= x;
 int T;
 cin >> T;
 LUDecomposition lu(A.pow(T).to_vec(N, N));
 auto c= lu.linear_equations(v);
 if (c.empty()) {
  cout << "none" << '\n';
 } else if (!lu.kernel().empty()) {
  cout << "ambiguous" << '\n';
 } else {
  for (int i= 0; i < N; i++) cout << c[i] << " \n"[i == N - 1];
 }
 return 0;
}