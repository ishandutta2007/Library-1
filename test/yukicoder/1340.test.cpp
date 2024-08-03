// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1340
#include <iostream>
#include "src/LinearAlgebra/Matrix.hpp"
#include "src/Math/SemiRing.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Rig= BitwiseOrAndRig<bool>;
 long long N, M, T;
 cin >> N >> M >> T;
 Matrix<Rig> A(N, N);
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