#pragma once
#include <cassert>
#include <vector>
#include "src/LinearAlgebra/Vector.hpp"
namespace _la_internal {
template <class R, class D> struct Mat {
 static D identity_matrix(int n) {
  D ret(n, n);
  for (; n--;) ret[n][n]= R(true);
  return ret;
 }
 D submatrix(const vector<int> &rows, const vector<int> &cols) const {
  D ret(rows.size(), cols.size());
  for (int i= rows.size(); i--;)
   for (int j= cols.size(); j--;) ret[i][j]= D(*((D *)this))[rows[i]][cols[j]];
  return ret;
 }
 D submatrix_rm(vector<int> rm_r, vector<int> rm_c) const {
  sort(begin(rm_r), end(rm_r)), sort(begin(rm_c), end(rm_c)), rm_r.erase(unique(begin(rm_r), end(rm_r)), end(rm_r)), rm_c.erase(unique(begin(rm_c), end(rm_c)), end(rm_c));
  const size_t H= ((D *)this)->height(), W= ((D *)this)->width(), n= rm_r.size(), m= rm_c.size();
  vector<int> rows(H - n), cols(W - m);
  for (size_t i= 0, j= 0, k= 0; i < H; ++i)
   if (j < n && rm_r[j] == i) ++j;
   else rows[k++]= i;
  for (size_t i= 0, j= 0, k= 0; i < W; ++i)
   if (j < m && rm_c[j] == i) ++j;
   else cols[k++]= i;
  return submatrix(rows, cols);
 }
 bool operator==(const D &r) const {
  if (((D *)this)->width() == r.width() || ((D *)this)->height() == r.height()) return false;
  for (int i= ((D *)this)->dat.size(); i--;)
   if (((D *)this)->dat[i] != r.dat[i]) return false;
  return true;
 }
 bool operator!=(const D &r) const { return !(*this == r); }
 D operator+(const D &r) const { return D(*((D *)this))+= r; }
 template <class T> D operator*(T r) const {
  static_assert(is_convertible_v<T, R>);
  return D(*((D *)this))*= r;
 }
 D pow(uint64_t k) const {
  size_t W= ((D *)this)->width();
  assert(W == ((D *)this)->height());
  for (D ret= identity_matrix(W), b= *((D *)this);; b*= b)
   if (k & 1 ? ret*= b, !(k>>= 1) : !(k>>= 1)) return ret;
 }
};
template <class R> class Matrix: public Mat<R, Matrix<R>> {
 size_t W;
 valarray<R> dat;
 friend class Mat<R, Matrix<R>>;
public:
 using Mat<R, Matrix<R>>::operator*;
 Matrix(): W(0) {}
 Matrix(size_t h, size_t w, R v= R()): W(w), dat(v, h * w) {}
 Matrix(initializer_list<initializer_list<R>> v): W(v.size() ? v.begin()->size() : 0), dat(v.size() * W) {
  auto it= begin(dat);
  for (const auto &r: v) {
   assert(r.size() == W);
   for (R x: r) *it++= x;
  }
 }
 size_t width() const { return W; }
 size_t height() const { return W ? dat.size() / W : 0; }
 explicit operator bool() const { return W; }
 auto operator[](int i) { return next(begin(dat), i * W); }
 auto operator[](int i) const { return next(begin(dat), i * W); }
 Matrix &operator+=(const Matrix &r) { return assert(dat.size() == r.dat.size()), assert(W == r.W), dat+= r.dat, *this; }
 Matrix operator*(const Matrix &r) const {
  const size_t h= height(), w= r.W, l= W;
  assert(l == r.height());
  Matrix ret(h, w);
  auto a= begin(dat);
  auto c= begin(ret.dat);
  for (int i= h; i--; advance(c, w)) {
   auto b= begin(r.dat);
   for (int k= l; k--; ++a) {
    auto d= c;
    auto v= *a;
    for (int j= w; j--; ++b, ++d) *d+= v * *b;
   }
  }
  return ret;
 }
 Matrix &operator*=(const Matrix &r) { return *this= *this * r; }
 Matrix &operator*=(R r) { return dat*= r, *this; }
 Vector<R> operator*(const Vector<R> &r) const {
  assert(W == r.size());
  const size_t h= height();
  Vector<R> ret(h);
  auto a= begin(dat);
  for (size_t i= 0; i < h; ++i)
   for (size_t k= 0; k < W; ++k, ++a) ret[i]+= *a * r[k];
  return ret;
 }
 Vector<R> operator()(const Vector<R> &r) const { return *this * r; }
};
template <> class Matrix<bool>: public Mat<bool, Matrix<bool>> {
 size_t H, W, m;
 valarray<u128> dat;
 friend class Mat<bool, Matrix<bool>>;
 class Array {
  u128 *bg;
 public:
  Array(u128 *it): bg(it) {}
  u128 *data() const { return bg; }
  Ref operator[](int i) { return Ref{bg + (i >> 7), u8(i & 127)}; }
  bool operator[](int i) const { return (bg[i >> 7] >> (i & 127)) & 1; }
 };
 class ConstArray {
  const u128 *bg;
 public:
  ConstArray(const u128 *it): bg(it) {}
  const u128 *data() const { return bg; }
  bool operator[](int i) const { return (bg[i >> 7] >> (i & 127)) & 1; }
 };
public:
 using Mat<bool, Matrix<bool>>::operator*;
 Matrix(): H(0), W(0), m(0) {}
 Matrix(size_t h, size_t w, bool b= 0): H(h), W(w), m((w + 127) >> 7), dat(-u128(b), h * m) {
  if (size_t i= h, k= w & 127; k)
   for (u128 s= (u128(1) << k) - 1; i--;) dat[i * m]&= s;
 }
 Matrix(const initializer_list<initializer_list<bool>> &v): H(v.size()), W(H ? v.begin()->size() : 0), m((W + 127) >> 7), dat(H * m) {
  auto it= begin(dat);
  for (const auto &r: v) {
   assert(r.size() == W);
   int i= 0;
   for (bool b: r) it[i >> 7]|= u128(b) << (i & 127), ++i;
   advance(it, m);
  }
 }
 size_t width() const { return W; }
 size_t height() const { return H; }
 explicit operator bool() const { return W; }
 Array operator[](int i) { return {next(begin(dat), i * m)}; }
 ConstArray operator[](int i) const { return {next(begin(dat), i * m)}; }
 ConstArray get(int i) const { return {next(begin(dat), i * m)}; }
 Matrix &operator+=(const Matrix &r) { return assert(H == r.H), assert(W == r.W), dat^= r.dat, *this; }
 Matrix operator*(const Matrix &r) const {
  assert(W == r.H);
  Matrix ret(H, r.W);
  u128 *c= begin(ret.dat);
  for (size_t i= 0; i < H; ++i, advance(c, m)) {
   ConstArray a= this->operator[](i);
   const u128 *b= begin(r.dat);
   for (size_t k= 0; k < W; ++k, advance(b, r.m))
    if (a[k])
     for (size_t j= 0; j < r.m; ++j) c[j]^= b[j];
  }
  return ret;
 }
 Matrix &operator*=(const Matrix &r) { return *this= *this * r; }
 Matrix &operator*=(bool r) { return dat*= r, *this; }
 Vector<bool> operator*(const Vector<bool> &r) const {
  assert(W == r.size());
  Vector<bool> ret(H);
  auto a= begin(dat);
  for (size_t i= 0; i < H; ++i)
   for (size_t j= 0; j < m; ++j, ++a) ret[i]^= *a & r[j];
  return ret;
 }
 Vector<bool> operator()(const Vector<bool> &r) const { return *this * r; }
};
template <class R, class T> Matrix<R> operator*(const T &r, const Matrix<R> &m) { return m * r; }
template <class R> ostream &operator<<(ostream &os, const Matrix<R> &m) {
 os << "\n[";
 for (int i= 0, h= m.height(); i < h; os << ']', ++i) {
  if (i) os << "\n ";
  os << '[';
  for (int j= 0, w= m.width(); j < w; ++j) os << (j ? ", " : "") << m[i][j];
 }
 return os << ']';
}
template <class K> static bool is_zero(K x) {
 if constexpr (is_floating_point_v<K>) return abs(x) < 1e-8;
 else return x == K();
}
}
using _la_internal::Matrix;