// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1421
// competitive-verifier: TLE 5
// competitive-verifier: MLE 256
#include <iostream>
#include <vector>
#include "mylib/algebra/LU_Decomposition.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, M;
 cin >> N >> M;
 Matrix<bool> A(M, N);
 vector<unsigned> Y(M);
 for(int i= 0; i < M; ++i) {
  int a;
  cin >> a;
  for(int j= 0; j < a; ++j) {
   int b;
   cin >> b;
   A[i][b - 1]= 1;
  }
  cin >> Y[i];
 }
 LU_Decomposition lu(A);
 vector<unsigned> X(N, 0);
 for(int bit= 0; bit < 30; ++bit) {
  Vector<bool> b(M);
  for(int i= 0; i < M; ++i) b[i]= (Y[i] >> bit) & 1;
  Vector<bool> x= lu.linear_equations(b);
  if(x.size() == 0) { cout << -1 << '\n'; return 0; }
  for(int j= 0; j < N; ++j)
   if(x[j]) X[j]|= 1u << bit;
 }
 for(int j= 0; j < N; ++j) cout << X[j] << '\n';
 return 0;
}
