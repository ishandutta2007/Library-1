// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1340
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include "src/LinearAlgebra/Matrix.hpp"
#include "src/Math/Algebra.hpp"
using namespace std;
struct Rig {
 using T= bool;
 static constexpr T o= false, i= true;
 static T add(T a, T b) { return a | b; }
 static T mul(T a, T b) { return a & b; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 long long N, M, T;
 cin >> N >> M >> T;
 Matrix<Algebra<Rig>> A(N, N);
 while (M--) {
  int a, b;
  cin >> a >> b;
  A[b][a]= 1;
 }
 auto B= A.pow(T);
 int ans= 0;
 for (int i= 0; i < N; i++) ans+= B[i][0].x;
 cout << ans << '\n';
 return 0;
}