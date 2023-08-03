#pragma once
#include <cassert>
#include "src/LinearAlgebra/Vector.hpp"
namespace la_internal {
template <class R> class Matrix {
public:
 size_t W;
 valarray<R> dat;
public:
 static Matrix identity_matrix(int n) {
  Matrix ret(n, n);
  return ret.dat[slice(0, n, n + 1)]= R(true), ret;
 }
 Matrix(): W(0) {}
 Matrix(size_t h, size_t w, R v= R()): W(w), dat(v, h * w) {}
 size_t width() const { return W; }
 size_t height() const { return W ? dat.size() / W : 0; }
 explicit operator bool() const { return W; }
 auto operator[](int i) { return next(begin(dat), i * W); }
 auto operator[](int i) const { return next(begin(dat), i * W); }
 Matrix submatrix(const vector<int> &rows, const vector<int> &cols) const {
  Matrix ret(rows.size(), cols.size());
  for (int i= rows.size(); i--;)
   for (int j= cols.size(); j--;) ret[i][j]= (*this)[rows[i]][cols[j]];
  return ret;
 }
 bool operator==(const Matrix &r) const {
  if (W != r.W || dat.size() != r.dat.size()) return false;
  for (int i= dat.size(); i--;)
   if (dat[i] != r.dat[i]) return false;
  return true;
 }
 bool operator!=(const Matrix &r) const { return !(*this == r); }
 Matrix &operator+=(const Matrix &r) { return assert(dat.size() == r.dat.size()), assert(W == r.W), dat+= r.dat, *this; }
 Matrix operator+(const Matrix &r) const { return Matrix(*this)+= r; }
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
 Matrix operator*(R r) const { return Matrix(*this)*= r; }
 Matrix &operator*=(const DiagonalMatrix<R> &r) {
  assert(W == r.size());
  const size_t h= height();
  auto a= begin(dat);
  for (int i= 0; i < h; ++i)
   for (int j= 0; j < W; ++j, ++a) *a*= r[j];
  return *this;
 }
 Matrix operator*(const DiagonalMatrix<R> &r) const { return Matrix(*this)*= r; }
 friend Matrix operator*(const DiagonalMatrix<R> &l, Matrix r) {
  const size_t h= r.height();
  assert(h == l.size());
  auto a= begin(r.dat);
  for (int i= 0; i < h; ++i) {
   auto v= l[i];
   for (int j= 0; j < r.W; ++j, ++a) *a*= v;
  }
  return r;
 }
 Vector<R> operator*(const Vector<R> &r) const {
  assert(W == r.size());
  const size_t h= height();
  Vector<R> ret(h);
  auto a= begin(dat);
  for (int i= 0; i < h; ++i)
   for (int k= 0; k < W; ++k, ++a) ret[i]+= *a * r[k];
  return ret;
 }
 Matrix pow(uint64_t k) const {
  assert(W * W == dat.size());
  for (auto ret= identity_matrix(W), b= *this;; b*= b)
   if (k & 1 ? ret*= b, !(k>>= 1) : !(k>>= 1)) return ret;
 }
};
template <> class Matrix<bool> {
 size_t H, W, m;
 valarray<u128> dat;
 class Array {
  u128 *bg;
 public:
  Array(u128 *it): bg(it) {}
  u128 *data() const { return bg; }
  Ref operator[](int i) {
   u128 *ref= bg + (i >> 7);
   u8 j= i & 127;
   bool val= (*ref >> j) & 1;
   return Ref{ref, j, val};
  }
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
 static Matrix identity_matrix(int n) {
  Matrix ret(n, n);
  for (; n--;) ret[n][n]= 1;
  return ret;
 }
 Matrix(): H(0), W(0), m(0) {}
 Matrix(size_t h, size_t w): H(h), W(w), m((w + 127) >> 7), dat(u128(0), h * m) {}
 size_t width() const { return W; }
 size_t height() const { return H; }
 explicit operator bool() const { return W; }
 Array operator[](int i) { return {next(begin(dat), i * m)}; }
 ConstArray operator[](int i) const { return {next(begin(dat), i * m)}; }
 ConstArray get(int i) const { return {next(begin(dat), i * m)}; }
 Matrix submatrix(const vector<int> &rows, const vector<int> &cols) const {
  Matrix ret(rows.size(), cols.size());
  for (int i= rows.size(); i--;)
   for (int j= cols.size(); j--;) ret[i][j]= (*this)[rows[i]][cols[j]];
  return ret;
 }
 bool operator==(const Matrix &r) const { return W == r.W && H == r.H && (dat == r.dat).min(); }
 bool operator!=(const Matrix &r) const { return W != r.W || H != r.H || (dat != r.dat).max(); }
 Matrix &operator+=(const Matrix &r) { return assert(H == r.H), assert(W == r.W), dat^= r.dat, *this; }
 Matrix operator+(const Matrix &r) const { return Matrix(*this)+= r; }
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
 Vector<bool> operator*(const Vector<bool> &r) const {
  assert(W == r.size());
  Vector<bool> ret(H);
  auto a= begin(dat);
  for (size_t i= 0; i < H; ++i)
   for (size_t j= 0; j < m; ++j, ++a) ret[i]^= *a & r[j];
  return ret;
 }
 Matrix pow(uint64_t k) const {
  assert(W == H);
  for (auto ret= identity_matrix(W), b= *this;; b*= b)
   if (k & 1 ? ret*= b, !(k>>= 1) : !(k>>= 1)) return ret;
 }
};
template <class K> static bool is_zero(K x) {
 if constexpr (is_floating_point_v<K>) return abs(x) < 1e-8;
 else return x == K();
}
}
using la_internal::Matrix;