// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc236/tasks/abc236_g
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// (min,max)半環
#include <iostream>
#include "src/Math/Algebra.hpp"
#include "src/LinearAlgebra/Matrix.hpp"
using namespace std;
struct M {
 using T= int;
 static constexpr T o= 1 << 30, i= 0;
 static T add(T a, T b) { return min(a, b); }
 static T mul(T a, T b) { return max(a, b); }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using R= Algebra<M>;
 int N, T, L;
 cin >> N >> T >> L;
 Matrix<R> A(N, N);
 for (int t= 1; t <= T; t++) {
  int u, v;
  cin >> u >> v, u--, v--;
  A[v][u]= t;
 }
 A= A.pow(L);
 for (int i= 0; i < N; i++) cout << (A[i][0] == R(false) ? -1 : A[i][0]) << " \n"[i == N - 1];
 return 0;
}