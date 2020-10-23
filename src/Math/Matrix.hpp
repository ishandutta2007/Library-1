#pragma once
#include <bits/stdc++.h>
/**
 * @title 行列
 * @category 数学
 *  Gauss_Jordan(A,B) 拡大係数行列に対するガウスジョルダン法
 *  linear_equations(A,b) 返り値 {解のうちの一つ,解空間の基底ベクトル}
 */

// BEGIN CUT HERE

template <typename K>
struct Matrix {
 private:
  std::vector<std::vector<K>> a;

 public:
  Matrix() {}
  Matrix(std::size_t n, std::size_t m) : a(n, std::vector<K>(m, 0)) {}
  Matrix(std::size_t n) : Matrix(n, n) {}
  Matrix(std::vector<std::vector<K>> a) : a(a) {}
  std::size_t height() const { return a.size(); }
  std::size_t width() const { return a[0].size(); }
  inline const std::vector<K> &operator[](std::size_t k) const { return a[k]; }
  inline std::vector<K> &operator[](std::size_t k) { return a[k]; }
  static Matrix diag(std::vector<K> v) {
    Matrix mat(v.size());
    for (std::size_t i = 0; i < v.size(); i++) mat[i][i] = v[i];
    return mat;
  }
  static Matrix I(std::size_t n, K e = {1}) {
    return diag(std::vector<K>(n, e));
  }
  Matrix &operator+=(const Matrix &b) {
    std::size_t n = height(), m = width();
    for (std::size_t i = 0; i < n; i++)
      for (std::size_t j = 0; j < m; j++) (*this)[i][j] += b[i][j];
    return (*this);
  }
  Matrix &operator-=(const Matrix &b) {
    std::size_t n = height(), m = width();
    for (std::size_t i = 0; i < n; i++)
      for (std::size_t j = 0; j < m; j++) (*this)[i][j] -= b[i][j];
    return (*this);
  }
  Matrix &operator*=(const Matrix &b) {
    std::size_t n = height(), m = width(), l = b.width();
    assert(m == b.height());
    std::vector<std::vector<K>> c(n, std::vector<K>(l, 0));
    for (std::size_t i = 0; i < n; i++)
      for (std::size_t j = 0; j < l; j++)
        for (std::size_t k = 0; k < m; k++) c[i][j] += (*this)[i][k] * b[k][j];
    a.swap(c);
    return (*this);
  }
  Matrix operator+(const Matrix &b) const { return (Matrix(*this) += b); }
  Matrix operator-(const Matrix &b) const { return (Matrix(*this) -= b); }
  Matrix operator*(const Matrix &b) const { return (Matrix(*this) *= b); }
  Matrix pow(uint64_t n, K e = {1}) {
    Matrix ret = I(height(), e);
    for (Matrix base = *this; n; n >>= 1, base *= base)
      if (n & 1) ret *= base;
    return ret;
  }
  std::vector<K> operator*(const std::vector<K> &v) {
    std::size_t n = height(), m = width();
    assert(m == v.size());
    std::vector<K> ret(n);
    for (std::size_t i = 0; i < n; i++)
      for (std::size_t j = 0; j < m; j++) ret[i] += (*this)[i][j] * v[j];
    return ret;
  }
  bool operator==(const Matrix &b) const { return a == b.a; }
  template <typename T>
  using ET = std::enable_if<std::is_floating_point<T>::value>;
  template <typename T>
  using EF = std::enable_if<!std::is_floating_point<T>::value>;
  template <typename T, typename ET<T>::type * = nullptr>
  static bool is_zero(T x) {
    return std::abs(x) < 1e-8;
  }
  template <typename T, typename EF<T>::type * = nullptr>
  static bool is_zero(T x) {
    return x == T(0);
  }
  template <typename T, typename ET<T>::type * = nullptr>
  static bool compare(T x, T y) {
    return std::abs(x) < std::abs(y);
  }
  template <typename T, typename EF<T>::type * = nullptr>
  static bool compare(T x, T y) {
    (void)x;
    return y != T(0);
  }
  // O(nm(m+l))
  static std::pair<Matrix, Matrix> Gauss_Jordan(const Matrix &a,
                                                const Matrix &b) {
    std::size_t n = a.height(), m = a.width(), l = b.width();
    Matrix c(n, m + l);
    for (std::size_t i = 0; i < n; i++)
      for (std::size_t j = 0; j < m; j++) c[i][j] = a[i][j];
    for (std::size_t i = 0; i < n; i++)
      for (std::size_t j = 0; j < l; j++) c[i][j + m] = b[i][j];
    for (std::size_t j = 0, d = 0; j < m && d < n; j++) {
      int p = d;
      for (std::size_t i = d + 1; i < n; i++)
        if (compare(c[p][j], c[i][j])) p = i;
      if (is_zero(c[p][j])) continue;
      std::swap(c[p], c[d]);
      K invc = K(1) / c[d][j];
      for (std::size_t k = j; k < m + l; k++) c[d][k] *= invc;
      for (std::size_t i = 0; i < n; i++) {
        if (i == d) continue;
        for (int k = m + l - 1; k >= (int)j; k--) c[i][k] -= c[i][j] * c[d][k];
      }
      d++;
    }
    Matrix reta(n, m), retb(n, l);
    for (std::size_t i = 0; i < n; i++)
      for (std::size_t j = 0; j < m; j++) reta[i][j] = c[i][j];
    for (std::size_t i = 0; i < n; i++)
      for (std::size_t j = 0; j < l; j++) retb[i][j] = c[i][j + m];
    return std::make_pair(reta, retb);
  }
  // O(nm^2)
  static std::pair<std::vector<K>, std::vector<std::vector<K>>>
  linear_equations(const Matrix &a, const std::vector<K> &b) {
    std::size_t n = a.height(), m = a.width();
    Matrix B(n, 1);
    for (std::size_t i = 0; i < n; i++) B[i][0] = b[i];
    auto p = Gauss_Jordan(a, B);
    std::vector<int> jdx(n, -1), idx(m, -1);
    for (std::size_t i = 0, j; i < n; i++) {
      for (j = 0; j < m; j++)
        if (!is_zero(p.first[i][j])) {
          jdx[i] = j, idx[j] = i;
          break;
        }
      if (j == m && !is_zero(p.second[i][0]))
        return std::make_pair(std::vector<K>(),
                              std::vector<std::vector<K>>());  // no solutions
    }
    std::vector<K> c(m);
    std::vector<std::vector<K>> d;
    for (std::size_t j = 0; j < m; j++) {
      if (idx[j] != -1)
        c[j] = p.second[idx[j]][0];
      else {
        std::vector<K> v(m);
        v[j] = 1;
        for (std::size_t i = 0; i < n; i++)
          if (jdx[i] != -1) v[jdx[i]] = -p.first[i][j];
        d.emplace_back(v);
      }
    }
    return std::make_pair(c, d);
  }
  // O(n^3)
  K det() const {
    int n = height();
    Matrix A(a);
    K ret(1);
    for (int i = 0; i < n; i++) {
      int p = i;
      for (int j = i + 1; j < n; j++)
        if (compare(A[p][i], A[j][i])) p = j;
      if (is_zero(A[p][i])) return 0;
      if (p != i) ret = -ret;
      std::swap(A[p], A[i]);
      ret *= A[i][i];
      K inva = K(1) / A[i][i];
      for (int j = i + 1; j < n; j++)
        for (int k = n - 1; k >= i; k--) A[j][k] -= inva * A[j][i] * A[i][k];
    }
    return ret;
  }
};