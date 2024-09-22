#pragma once
#include <cassert>
#include <vector>
#include "src/LinearAlgebra/Vector.hpp"
namespace _la_internal {
template <class R, class D> struct Mat {
 Mat(): W(0) {}
 Mat(size_t h, size_t w): W(w), dat(h * w) {}
 Mat(size_t h, size_t w, R v): W(w), dat(v, h * w) {}
 Mat(initializer_list<initializer_list<R>> v): W(v.size() ? v.begin()->size() : 0), dat(v.size() * W) {
  auto it= begin(dat);
  for (const auto &r: v) {
   assert(r.size() == W);
   for (R x: r) *it++= x;
  }
 }
 size_t width() const { return W; }
 size_t height() const { return W ? dat.size() / W : 0; }
 auto operator[](int i) { return begin(dat) + i * W; }
 auto operator[](int i) const { return begin(dat) + i * W; }
protected:
 size_t W;
 valarray<R> dat;
 void add(const Mat &r) { assert(dat.size() == r.dat.size()), assert(W == r.W), dat+= r.dat; }
 D mul(const Mat &r) const {
  const size_t h= height(), w= r.W, l= W;
  assert(l == r.height());
  D ret(h, w);
  auto a= begin(dat);
  auto c= begin(ret.dat);
  for (int i= h; i--; c+= w) {
   auto b= begin(r.dat);
   for (int k= l; k--; ++a) {
    auto d= c;
    auto v= *a;
    for (int j= w; j--; ++b, ++d) *d+= v * *b;
   }
  }
  return ret;
 }
 Vector<R> mul(const Vector<R> &r) const {
  assert(W == r.size());
  const size_t h= height();
  Vector<R> ret(h);
  auto a= begin(dat);
  for (size_t i= 0; i < h; ++i)
   for (size_t k= 0; k < W; ++k, ++a) ret[i]+= *a * r[k];
  return ret;
 }
};
template <class D> struct Mat<bool, D> {
 struct Array {
  u128 *bg;
  Array(u128 *it): bg(it) {}
  Ref operator[](int i) { return Ref{bg + (i >> 7), u8(i & 127)}; }
  bool operator[](int i) const { return (bg[i >> 7] >> (i & 127)) & 1; }
 };
 struct ConstArray {
  const u128 *bg;
  ConstArray(const u128 *it): bg(it) {}
  bool operator[](int i) const { return (bg[i >> 7] >> (i & 127)) & 1; }
 };
 Mat(): H(0), W(0), m(0) {}
 Mat(size_t h, size_t w): H(h), W(w), m((w + 127) >> 7), dat(h * m) {}
 Mat(size_t h, size_t w, bool b): H(h), W(w), m((w + 127) >> 7), dat(-u128(b), h * m) {
  if (size_t i= h, k= w & 127; k)
   for (u128 s= (u128(1) << k) - 1; i--;) dat[i * m]&= s;
 }
 Mat(const initializer_list<initializer_list<bool>> &v): H(v.size()), W(H ? v.begin()->size() : 0), m((W + 127) >> 7), dat(H * m) {
  auto it= begin(dat);
  for (const auto &r: v) {
   assert(r.size() == W);
   int i= 0;
   for (bool b: r) it[i >> 7]|= u128(b) << (i & 127), ++i;
   it+= m;
  }
 }
 size_t width() const { return W; }
 size_t height() const { return H; }
 Array operator[](int i) { return {begin(dat) + i * m}; }
 ConstArray operator[](int i) const { return {begin(dat) + i * m}; }
 ConstArray get(int i) const { return {begin(dat) + i * m}; }
protected:
 size_t H, W, m;
 valarray<u128> dat;
 void add(const Mat &r) { assert(H == r.H), assert(W == r.W), dat^= r.dat; }
 D mul(const Mat &r) const {
  assert(W == r.H);
  D ret(H, r.W);
  valarray<u128> tmp(r.m << 8);
  auto y= begin(r.dat);
  for (size_t l= 0; l < W; l+= 8) {
   auto t= begin(tmp) + r.m;
   for (int i= 0, n= min<size_t>(8, W - l); i < n; ++i, y+= r.m) {
    auto u= begin(tmp);
    for (int s= 1 << i; s--;) {
     auto z= y;
     for (int j= r.m; j--; ++u, ++t, ++z) *t= *u ^ *z;
    }
   }
   auto a= begin(dat) + (l >> 7);
   auto c= begin(ret.dat);
   for (int i= H; i--; a+= m) {
    auto u= begin(tmp) + ((*a >> (l & 127)) & 255);
    for (int j= r.m; j--; ++c, ++u) *c^= *u;
   }
  }
  return ret;
 }
 Vector<bool> mul(const Vector<bool> &r) const {
  assert(W == r.size());
  Vector<bool> ret(H);
  auto a= begin(dat);
  for (size_t i= 0; i < H; ++i) {
   u128 v= 0;
   for (size_t j= 0; j < m; ++j, ++a) v^= *a & r.dat[j];
   ret[i]= __builtin_parityll(v >> 64) ^ __builtin_parityll(u64(v));
  }
  return ret;
 }
};
template <class R> struct Matrix: public Mat<R, Matrix<R>> {
 using Mat<R, Matrix<R>>::Mat;
 explicit operator bool() const { return this->W; }
 static Matrix identity(int n) {
  Matrix ret(n, n);
  for (; n--;) ret[n][n]= R(true);
  return ret;
 }
 Matrix submatrix(const vector<int> &rows, const vector<int> &cols) const {
  Matrix ret(rows.size(), cols.size());
  for (int i= rows.size(); i--;)
   for (int j= cols.size(); j--;) ret[i][j]= (*this)[rows[i]][cols[j]];
  return ret;
 }
 Matrix submatrix_rm(vector<int> rows, vector<int> cols) const {
  sort(begin(rows), end(rows)), sort(begin(cols), end(cols)), rows.erase(unique(begin(rows), end(rows)), end(rows)), cols.erase(unique(begin(cols), end(cols)), end(cols));
  const int H= this->height(), W= this->width(), n= rows.size(), m= cols.size();
  vector<int> rs(H - n), cs(W - m);
  for (int i= 0, j= 0, k= 0; i < H; ++i)
   if (j < n && rows[j] == i) ++j;
   else rs[k++]= i;
  for (int i= 0, j= 0, k= 0; i < W; ++i)
   if (j < m && cols[j] == i) ++j;
   else cs[k++]= i;
  return submatrix(rs, cs);
 }
 bool operator==(const Matrix &r) const {
  if (this->width() != r.width() || this->height() != r.height()) return false;
  for (int i= this->dat.size(); i--;)
   if (this->dat[i] != r.dat[i]) return false;
  return true;
 }
 bool operator!=(const Matrix &r) const { return !(*this == r); }
 Matrix &operator*=(const Matrix &r) { return *this= this->mul(r); }
 Matrix operator*(const Matrix &r) const { return this->mul(r); }
 Matrix &operator*=(R r) { return this->dat*= r, *this; }
 template <class T> Matrix operator*(T r) const {
  static_assert(is_convertible_v<T, R>);
  return Matrix(*this)*= r;
 }
 Matrix &operator+=(const Matrix &r) { return this->add(r), *this; }
 Matrix operator+(const Matrix &r) const { return Matrix(*this)+= r; }
 Vector<R> operator*(const Vector<R> &r) const { return this->mul(r); }
 Vector<R> operator()(const Vector<R> &r) const { return this->mul(r); }
 Matrix pow(uint64_t k) const {
  size_t W= this->width();
  assert(W == this->height());
  for (Matrix ret= identity(W), b= *this;; b*= b)
   if (k & 1 ? ret*= b, !(k>>= 1) : !(k>>= 1)) return ret;
 }
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