// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/2530
// F_2 の線形方程式・カーネル
#include <iostream>
#include "src/LinearAlgebra/LU_Decomposition.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int R, C;
 cin >> R >> C;
 Matrix<bool> A(R * C, R * C);
 Vector<bool> b(R * C);
 for (int i= 0; i < R; i++)
  for (int j= 0, x; j < C; j++) cin >> x, b[i * C + j]= x;
 for (int i= 0; i < R; i++)
  for (int j= 0; j < C; j++)
   for (int di= -1; di <= 1; di++)
    for (int dj= -1; dj <= 1; dj++)
     if (di != 0 || dj != 0)
      for (int ni= i, nj= j; 0 <= ni && ni < R && 0 <= nj && nj < C; ni+= di, nj+= dj) A[i * C + j][ni * C + nj]= 1;
 LU_Decomposition lu(A);
 if (lu.linear_equations(b).size()) cout << ModInt<int(1e9 + 9)>(2).pow(lu.kernel().size()) << '\n';
 else cout << 0 << '\n';
 return 0;
}