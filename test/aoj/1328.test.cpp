// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/ICPC/Regional/1328
// double型の行列の連立一次方程式のverify
#include <iostream>
#include <vector>
#include <algorithm>
#include "src/LinearAlgebra/LU_Decomposition.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 for (int d; cin >> d && d;) {
  vector m(d + 3, Vector<double>(1, d + 1));
  vector<double> v(d + 3);
  for (int i= 0; i < d + 3; ++i) cin >> v[i];
  for (int i= 0; i < d + 3; ++i)
   for (int j= 1; j <= d; ++j) m[i][j]= m[i][j - 1] * i;
  int ans= d + 2;
  [&]() {
   for (int k= 0; k + 1 < d + 3; ++k)
    for (int l= k + 1; l < d + 3; ++l) {
     Matrix<double> A(d + 1, d + 1);
     Vector<double> b(d + 1);
     int ii= 0;
     for (int i= 0; i < d + 3; ++i)
      if (i != k && i != l) {
       b[ii]= v[i];
       for (int j= d + 1; j--;) A[ii][j]= m[i][j];
       ii++;
      }
     auto c= LU_Decomposition(A).linear_equations(b);
     if (abs(dot(c, m[k]) - v[k]) < 1e-5) {
      ans= l;
      return;
     }
     if (abs(dot(c, m[l]) - v[l]) < 1e-5) {
      ans= k;
      return;
     }
    }
  }();
  cout << ans << '\n';
 }
 return 0;
}