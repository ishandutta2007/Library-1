#pragma once
#include <vector>
#include <tuple>
#include <cassert>
#include "src/LinearAlgebra/Vector.hpp"
template <class R> class SparseMatrix {
 std::vector<std::tuple<size_t, size_t, R>> dat;
 size_t H, W;
public:
 SparseMatrix(size_t h, size_t w): H(h), W(w) {}
 size_t width() const { return W; }
 size_t height() const { return H; }
 void add_component(size_t i, size_t j, R v) { dat.emplace_back(i, j, v); }
 Vector<R> operator*(const Vector<R> &r) const {
  assert(W == r.size());
  Vector<R> ret(H);
  for (auto [i, j, v]: dat) ret[i]+= v * r[j];
  return ret;
 }
 SparseMatrix &operator*=(const DiagonalMatrix<R> &r) {
  assert(W == r.size());
  for (auto &[i, j, v]: dat) v*= r[j];
  return *this;
 }
 SparseMatrix operator*(const DiagonalMatrix<R> &r) const { return SparseMatrix(*this)*= r; }
 friend SparseMatrix operator*(const DiagonalMatrix<R> &l, SparseMatrix r) {
  assert(r.H == l.size());
  for (auto &[i, j, v]: r.dat) v*= l[i];
  return r;
 }
};