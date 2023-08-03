#pragma once
#include <vector>
#include <algorithm>
#include "src/LinearAlgebra/Matrix.hpp"
template <class K> Matrix<K> hessenberg(const Matrix<K> &A) {
 size_t n= A.width();
 assert(n == A.height());
 auto ret= A;
 for (size_t j= 0, i, r; j + 2 < n; ++j) {
  if (ret[j + 1][j] == K())
   for (i= j + 2; i < n; ++i)
    if (ret[i][j] != K()) {
     for (r= 0; r < n; ++r) std::swap(ret[j + 1][r], ret[i][r]);
     for (; r--;) std::swap(ret[r][j + 1], ret[r][i]);
     break;
    }
  if (K iv; ret[j + 1][j] != K())
   for (iv= K(1) / ret[j + 1][j], i= j + 2; i < n; ++i) {
    for (K m= ret[i][r= j] * iv; r < n; ++r) ret[i][r]-= m * ret[j + 1][r];
    for (; r--;) ret[r][j + 1]+= m * ret[r][i];
   }
 }
 return ret;
}
template <class K> std::vector<K> characteristic_polynomial(const Matrix<K> &A) {
 size_t n= A.width();
 assert(n == A.height());
 auto b= hessenberg(A);
 std::vector<K> fss((n + 1) * (n + 2) / 2);
 K *pr= fss.data(), *nx= pr, prod, tmp, s;
 fss[0]= 1;
 for (size_t i= 0; i < n; ++i, pr= nx) {
  prod= 1, tmp= -b[i][i], nx= pr + i + 1, std::copy_n(pr, i + 1, nx + 1);
  for (size_t k= 0; k <= i; ++k) nx[k]+= tmp * pr[k];
  for (size_t j= i, k; j--;)
   for (pr-= j + 1, s= (prod*= b[j + 1][j]) * -b[j][i], k= 0; k <= j; ++k) nx[k]+= s * pr[k];
 }
 return std::vector<K>(fss.begin() + n * (n + 1) / 2, fss.end());
}