#pragma once
#include <vector>
#include <algorithm>
#include "src/LinearAlgebra/Matrix.hpp"
template <class K> Matrix<K> hessenberg(Matrix<K> A, bool mint= false) {
 using la_internal::is_zero;
 const size_t n= A.width();
 assert(n == A.height());
 for (size_t j= 0, i, r; j + 2 < n; ++j) {
  if constexpr (std::is_floating_point_v<K>) {
   for (i= j + 1, r= j + 2; r < n; ++r)
    if (std::abs(A[i][j]) < std::abs(A[r][j])) i= r;
  } else
   for (i= r= j + 1; r < n; ++r)
    if (A[r][j] != K()) i= r, r= n;
  if (i != j + 1) {
   for (r= 0; r < n; ++r) std::swap(A[j + 1][r], A[i][r]);
   for (; r--;) std::swap(A[r][j + 1], A[r][i]);
  }
  if (is_zero(A[j + 1][j])) continue;
  if (K s, iv; mint) {
   for (i= j + 2; i < n; ++i)
    if (!is_zero(A[i][j])) {
     K m00= K(1), m01= K(), m10= K(), m11= K(1);
     for (uint64_t a= A[j + 1][j].val(), b= A[i][j].val(), t, l; b;) l= b, b= a - (t= a / b) * b, a= l, s= m10, m10= m00 - m10 * t, m00= s, s= m11, m11= m01 - m11 * t, m01= s;
     for (r= 0; r < n; ++r) s= m00 * A[j + 1][r] + m01 * A[i][r], A[i][r]= m10 * A[j + 1][r] + m11 * A[i][r], A[j + 1][r]= s;
     for (; r--;) s= m11 * A[r][j + 1] - m10 * A[r][i], A[r][i]= m00 * A[r][i] - m01 * A[r][j + 1], A[r][j + 1]= s;
    }
  } else {
   for (iv= K(1) / A[j + 1][j], i= j + 2; i < n; ++i)
    if (!is_zero(A[i][j])) {
     for (s= A[i][r= j] * iv; r < n; ++r) A[i][r]-= s * A[j + 1][r];
     for (; r--;) A[r][j + 1]+= s * A[r][i];
    }
  }
 }
 return A;
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
template <class K> std::vector<K> det_of_first_degree_poly_mat(Matrix<K> M0, Matrix<K> M1) {
 const size_t n= M0.height();
 assert(n == M1.height()), assert(n == M0.width()), assert(n == M1.width());
 size_t cnt= 0;
 K det= 1, v, iv;
 for (size_t p= 0, piv, r, i; p < n;) {
  if constexpr (std::is_floating_point_v<K>) {
   for (piv= p, r= p + 1; r < n; ++r)
    if (std::abs(M1[piv][p]) < std::abs(M1[r][p])) piv= r;
  } else
   for (piv= p; piv < n; ++piv)
    if (M1[piv][p] != K()) break;
  if (piv == n || la_internal::is_zero(M1[piv][p])) {
   if (++cnt > n) return std::vector<K>(n + 1);
   for (r= p; r--;)
    for (v= M1[r][p], M1[r][p]= K(), i= n; i--;) M0[i][p]-= v * M0[i][r];
   for (i= n; i--;) std::swap(M0[i][p], M1[i][p]);
   continue;
  }
  if (piv != p) {
   for (det*= -1, i= 0; i < n; ++i) std::swap(M0[p][i], M0[piv][i]);
   for (; i--;) std::swap(M1[p][i], M1[piv][i]);
  }
  for (det*= v= M1[p][p], iv= K(1) / v, i= 0; i < n; ++i) M0[p][i]*= iv;
  for (; i--;) M1[p][i]*= iv;
  for (r= n; r--;)
   if (r != p) {
    for (v= M1[r][p], i= 0; i < n; ++i) M0[r][i]-= v * M0[p][i];
    for (; i--;) M1[r][i]-= v * M1[p][i];
   }
  ++p;
 }
 auto poly= characteristic_polynomial(M0 * -1);
 poly.erase(poly.begin(), poly.begin() + cnt);
 for (auto &x: poly) x*= det;
 return poly.resize(n + 1), poly;
}