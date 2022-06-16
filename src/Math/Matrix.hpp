#pragma once
#include <bits/stdc++.h>
/**
 * @title 行列
 * @category 数学
 * 半環が載る
 */

// BEGIN CUT HERE

struct has_I_impl {
  template <class T>
  static auto check(T &&x) -> decltype(x.I(), std::true_type{});
  template <class T>
  static auto check(...) -> std::false_type;
};
template <class T>
class has_I : public decltype(has_I_impl::check<T>(std::declval<T>())) {};
template <class T>
inline constexpr bool has_I_v = has_I<T>::value;

template <class R, int N, int M>
struct Matrix : public std::array<std::array<R, M>, N> {
  static Matrix O() { return Matrix{}; }
  Matrix &operator+=(const Matrix &r) {
    for (int i = 0; i < N; i++)
      for (int j = 0; j < M; j++) (*this)[i][j] += r[i][j];
    return *this;
  }
  Matrix operator+(const Matrix &r) const { return Matrix(*this) += r; }
  template <int L>
  Matrix<R, N, L> operator*(const Matrix<R, M, L> &r) const {
    Matrix<R, N, L> ret;
    for (int i = 0; i < N; i++)
      for (int j = 0; j < L; j++)
        for (int k = 0; k < M; k++) ret[i][j] += (*this)[i][k] * r[k][j];
    return ret;
  }
  std::array<R, N> operator*(const std::array<R, M> &r) const {
    std::array<R, N> ret;
    for (int i = 0; i < N; i++)
      for (int j = 0; j < M; j++) ret[i] += (*this)[i][j] * r[j];
    return ret;
  }
  std::vector<std::vector<R>> to_vec(int n, int m) const {
    std::vector<std::vector<R>> ret(n, std::vector<R>(m));
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++) ret[i][j] = (*this)[i][j];
    return ret;
  }
};

template <int N, int M>
struct Matrix<bool, N, M> : public std::array<std::bitset<M>, N> {
  static Matrix O() { return Matrix{}; }
  Matrix &operator+=(const Matrix &r) {
    for (int i = 0; i < N; i++) (*this)[i] ^= r[i];
    return *this;
  }
  Matrix operator+(const Matrix &r) const { return Matrix(*this) += r; }
  template <int L>
  Matrix<bool, N, L> operator*(const Matrix<bool, M, L> &r) const {
    Matrix<bool, L, M> t;
    Matrix<bool, N, L> ret;
    for (int i = 0; i < M; i++)
      for (int j = 0; j < L; j++) t[j][i] = r[i][j];
    for (int i = 0; i < N; i++)
      for (int j = 0; j < L; j++) ret[i][j] = ((*this)[i] & t[j]).count() & 1;
    return ret;
  }
  std::bitset<N> operator*(const std::bitset<N> &r) const {
    std::bitset<N> ret;
    for (int i = 0; i < N; i++) ret[i] = ((*this)[i] & r).count() & 1;
    return ret;
  }
  std::vector<std::vector<bool>> to_vec(int n, int m) const {
    std::vector<std::vector<bool>> ret(n, std::vector<bool>(m));
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++) ret[i][j] = (*this)[i][j];
    return ret;
  }
};

template <class R, int N>
struct SquareMatrix : public Matrix<R, N, N> {
  using Matrix<R, N, N>::Matrix;
  SquareMatrix(Matrix<R, N, N> m) { *this = m; }
  static SquareMatrix I() {
    SquareMatrix ret;
    if constexpr (has_I_v<R>)
      for (int i = 0; i < N; i++) ret[i][i] = R::I();
    else
      for (int i = 0; i < N; i++) ret[i][i] = R(1);
    return ret;
  }
  SquareMatrix &operator=(const Matrix<R, N, N> &r) {
    for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++) (*this)[i][j] = r[i][j];
    return *this;
  }
  SquareMatrix &operator*=(const SquareMatrix &r) {
    return *this = (*this) * r;
  }
  SquareMatrix pow(std::uint64_t e) const {
    for (SquareMatrix ret = I(), b = *this;; b *= b)
      if (e & 1 ? ret *= b, !(e >>= 1) : !(e >>= 1)) return ret;
  }
};