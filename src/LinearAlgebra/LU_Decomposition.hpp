#pragma once
#include <type_traits>
#include <numeric>
#include <vector>
#include "src/LinearAlgebra/Matrix.hpp"
namespace la_internal {
template <class K> class LU_Decomposition {
 Matrix<K> dat;
 std::vector<size_t> perm, piv;
 bool sgn;
 size_t psz;
 static bool is_zero(K x) {
  if constexpr (std::is_floating_point_v<K>) return std::abs(x) < 1e-8;
  else return x == K();
 }
public:
 LU_Decomposition(const Matrix<K> &A): dat(A), perm(A.height()), sgn(false), psz(0) {
  const size_t h= A.height(), w= A.width();
  std::iota(perm.begin(), perm.end(), 0), piv.resize(std::min(w, h));
  for (size_t c= 0, pos; c < w && psz < h; ++c) {
   pos= psz;
   if constexpr (std::is_floating_point_v<K>) {
    for (size_t r= psz + 1; r < h; ++r)
     if (std::abs(dat[perm[pos]][c]) < std::abs(dat[perm[r]][c])) pos= r;
   } else if (is_zero(dat[perm[pos]][c]))
    for (size_t r= psz + 1; r < h; ++r)
     if (!is_zero(dat[perm[r]][c])) pos= r, r= h;
   if (is_zero(dat[perm[pos]][c])) continue;
   if (pos != psz) sgn= !sgn, std::swap(perm[pos], perm[psz]);
   const auto b= dat[perm[psz]];
   for (size_t r= psz + 1, i; r < h; ++r) {
    auto a= dat[perm[r]];
    K m= a[c] / b[c];
    for (a[c]= K(), a[psz]= m, i= c + 1; i < w; ++i) a[i]-= b[i] * m;
   }
   piv[psz++]= c;
  }
 }
 size_t rank() const { return psz; }
 bool is_regular() const { return rank() == dat.height() && rank() == dat.width(); }
 K det() const {
  assert(dat.height() == dat.width());
  K ret= sgn ? -1 : 1;
  for (size_t i= dat.width(); i--;) ret*= dat[perm[i]][i];
  return ret;
 }
 std::vector<Vector<K>> kernel() const {
  const size_t w= dat.width(), n= rank();
  std::vector ker(w - n, Vector<K>(w));
  for (size_t c= 0, i= 0; c < w; ++c) {
   if (i < n && piv[i] == c) ++i;
   else {
    auto a= ker[c - i];
    a[c]= 1;
    for (size_t r= i; r--;) a[r]= -dat[perm[r]][c];
    for (size_t j= i, k, r; j--;) {
     K x= a[j] / dat[perm[j]][k= piv[j]];
     for (a[j]= 0, a[k]= x, r= j; r--;) a[r]-= dat[perm[r]][k] * x;
    }
   }
  }
  return ker;
 }
 Vector<K> linear_equations(const Vector<K> &b) const {
  const size_t h= dat.height(), w= dat.width(), n= rank();
  assert(h == b.size());
  Vector<K> y(h), x(w);
  for (size_t c= 0; c < h; ++c)
   if (y[c]+= b[perm[c]]; c < w)
    for (size_t r= c + 1; r < h; ++r) y[r]-= y[c] * dat[perm[r]][c];
  for (size_t i= n; i < h; ++i)
   if (!is_zero(y[i])) return Vector<K>();  // no solution
  for (size_t i= n, r; i--;)
   for (x[piv[i]]= y[i] / dat[perm[i]][piv[i]], r= i; r--;) y[r]-= x[piv[i]] * dat[perm[r]][piv[i]];
  return x;
 }
 Matrix<K> inverse_matrix() const {
  if (!is_regular()) return Matrix<K>();  // no solution
  const size_t n= dat.width();
  Matrix<K> ret(n, n);
  for (size_t i= 0; i < n; ++i) {
   Vector<K> y(n);
   for (size_t c= 0; c < n; ++c)
    if (y[c]+= perm[c] == i; c < n && !is_zero(y[c]))
     for (size_t r= c + 1; r < n; ++r) y[r]-= y[c] * dat[perm[r]][c];
   for (size_t j= n; j--;) {
    K m= ret[j][i]= y[j] / dat[perm[j]][j];
    for (size_t r= j; r--;) y[r]-= m * dat[perm[r]][j];
   }
  }
  return ret;
 }
};
void add_upper(u128 *a, const u128 *b, size_t bg, size_t ed) {  //[bg,ed)
 if (bg >= ed) return;
 size_t s= bg >> 7;
 a[s]^= b[s] & -(u128(1) << (bg & 127));
 for (size_t i= (ed + 127) >> 7; --i > s;) a[i]^= b[i];
}
void add_lower(u128 *a, const u128 *b, size_t ed) {  //[0,ed)
 size_t s= ed >> 7;
 a[s]^= b[s] & ((u128(1) << (ed & 127)) - 1);
 for (size_t i= s; i--;) a[i]^= b[i];
}
void subst_lower(u128 *a, const u128 *b, size_t ed) {  //[0,ed)
 size_t s= ed >> 7;
 a[s]= b[s] & ((u128(1) << (ed & 127)) - 1);
 for (size_t i= s; i--;) a[i]= b[i];
}
bool any1_upper(const u128 *a, size_t bg, size_t ed) {  //[bg,ed)
 if (bg >= ed) return false;
 size_t s= bg >> 7;
 if (a[s] & -(u128(1) << (bg & 127))) return true;
 for (size_t i= (ed + 127) >> 7; --i > s;)
  if (a[i]) return true;
 return false;
}
template <> class LU_Decomposition<bool> {
 Matrix<bool> dat;
 std::vector<size_t> perm, piv;
 size_t psz;
public:
 LU_Decomposition(Matrix<bool> A): dat(A.width(), A.height()), perm(A.height()), psz(0) {
  const size_t h= A.height(), w= A.width();
  std::iota(perm.begin(), perm.end(), 0), piv.resize(std::min(w, h));
  for (size_t c= 0, pos; c < w && psz < h; ++c) {
   for (pos= psz; pos < h; ++pos)
    if (A.get(perm[pos])[c]) break;
   if (pos == h) continue;
   if (pos != psz) std::swap(perm[pos], perm[psz]);
   auto b= A.get(perm[psz]);
   for (size_t r= psz + 1; r < h; ++r) {
    auto a= A[perm[r]];
    if (bool m= a[c]; m) add_upper(a.data(), b.data(), c, w), a[psz]= 1;
   }
   piv[psz++]= c;
  }
  for (size_t j= w; j--;)
   for (size_t i= h; i--;) dat[j][i]= A.get(perm[i])[j];
 }
 size_t rank() const { return psz; }
 bool is_regular() const { return rank() == dat.height() && rank() == dat.width(); }
 bool det() const { return is_regular(); }
 std::vector<Vector<bool>> kernel() const {
  const size_t w= dat.height(), n= rank();
  std::vector ker(w - rank(), Vector<bool>(w));
  for (size_t c= 0, i= 0; c < w; ++c) {
   if (i < n && piv[i] == c) ++i;
   else {
    auto a= ker[c - i];
    subst_lower(a.data(), dat[c].data(), i), a[c]= 1;
    for (size_t j= i, k; j--;) {
     bool x= a[j];
     if (a[j]= 0, a[k= piv[j]]= x; x) add_lower(a.data(), dat[k].data(), j);
    }
   }
  }
  return ker;
 }
 Vector<bool> linear_equations(const Vector<bool> &b) const {
  const size_t h= dat.width(), w= dat.height(), n= rank();
  assert(h == b.size());
  Vector<bool> y(h), x(w);
  for (size_t c= 0; c < h; ++c)
   if (y[c]^= b[perm[c]]; c < w && y[c]) add_upper(y.data(), dat[c].data(), c + 1, h);
  if (any1_upper(y.data(), n, h)) return Vector<bool>();  // no solution
  for (size_t i= n; i--;)
   if ((x[piv[i]]= y[i])) add_lower(y.data(), dat[piv[i]].data(), i);
  return x;
 }
 Matrix<bool> inverse_matrix() const {
  if (!is_regular()) return Matrix<bool>();  // no solution
  const size_t n= dat.width();
  Matrix<bool> ret(n, n);
  for (size_t i= 0; i < n; ++i) {
   Vector<bool> y(n);
   for (size_t c= 0; c < n; ++c)
    if (y[c]^= perm[c] == i; c < n && y[c]) add_upper(y.data(), dat[c].data(), c, n);
   for (size_t j= n; j--;)
    if ((ret[j][i]= y[j])) add_lower(y.data(), dat[j].data(), j);
  }
  return ret;
 }
};
}
using la_internal::LU_Decomposition;