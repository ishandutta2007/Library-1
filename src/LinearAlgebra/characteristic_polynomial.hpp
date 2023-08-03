#pragma once
#include <vector>
#include <algorithm>
#include "src/LinearAlgebra/Matrix.hpp"
template <class K> Matrix<K> hessenberg(const Matrix<K> &A, bool mint= false) {
 size_t n= A.width();
 assert(n == A.height());
 auto ret= A;
 auto is_zero= [](K x) {
  if constexpr (std::is_floating_point_v<K>) return std::abs(x) < 1e-8;
  else return x == K();
 };
 for (size_t j= 0, i, r; j + 2 < n; ++j) {
  if constexpr (std::is_floating_point_v<K>) {
   for (i= j + 1, r= j + 2; r < n; ++r)
    if (std::abs(ret[i][j]) < std::abs(ret[r][j])) i= r;
  } else
   for (i= j + 1; i < n; ++i)
    if (ret[i][j] != K()) break;
  if (i != j + 1) {
   for (r= 0; r < n; ++r) std::swap(ret[j + 1][r], ret[i][r]);
   for (; r--;) std::swap(ret[r][j + 1], ret[r][i]);
  }
  if (is_zero(ret[j + 1][j])) continue;
  if (K s, iv; mint) {
   for (i= j + 2; i < n; ++i)
    if (!is_zero(ret[i][j])) {
     K m00= K(1), m01= K(), m10= K(), m11= K(1);
     for (uint64_t a= ret[j + 1][j].val(), b= ret[i][j].val(), t, l; b;) l= b, b= a - (t= a / b) * b, a= l, s= m10, m10= m00 - m10 * t, m00= s, s= m11, m11= m01 - m11 * t, m01= s;
     for (r= 0; r < n; ++r) s= m00 * ret[j + 1][r] + m01 * ret[i][r], ret[i][r]= m10 * ret[j + 1][r] + m11 * ret[i][r], ret[j + 1][r]= s;
     for (; r--;) s= m11 * ret[r][j + 1] - m10 * ret[r][i], ret[r][j + 1]= m00 * ret[r][i] - m01 * ret[r][j + 1], ret[r][i]= s;
    }
  } else {
   for (iv= K(1) / ret[j + 1][j], i= j + 2; i < n; ++i)
    if (!is_zero(ret[i][j])) {
     for (s= ret[i][r= j] * iv; r < n; ++r) ret[i][r]-= s * ret[j + 1][r];
     for (; r--;) ret[r][j + 1]+= s * ret[r][i];
    }
  }
 }
 return ret;
}
template <class K> std::vector<K> characteristic_polynomial(const Matrix<K> &A, bool mint= false) {
 size_t n= A.width(), i= 0, k, j;
 assert(n == A.height());
 auto b= hessenberg(A, mint);
 std::vector<K> fss((n + 1) * (n + 2) / 2);
 K *pr= fss.data(), *nx= pr, prod, tmp, s;
 for (fss[0]= 1; i < n; ++i, pr= nx) {
  for (prod= 1, tmp= -b[i][i], nx= pr + i + 1, std::copy_n(pr, i + 1, nx + 1), k= 0; k <= i; ++k) nx[k]+= tmp * pr[k];
  for (j= i; j--;)
   for (pr-= j + 1, s= (prod*= b[j + 1][j]) * -b[j][i], k= 0; k <= j; ++k) nx[k]+= s * pr[k];
 }
 return std::vector<K>(fss.begin() + n * (n + 1) / 2, fss.end());
}