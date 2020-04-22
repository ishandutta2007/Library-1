/**
 * @title 行列
 * @brief Gauss_Jordan(A,B) 拡大係数行列に対するガウスジョルダン法
 * @brief linear_equations(A,b) 返り値 {解のうちの一つ,解空間の基底ベクトル}
 * @category 数学
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

template <typename K>
struct Matrix {
 private:
  vector<vector<K>> a;

 public:
  Matrix() {}
  Matrix(size_t n, size_t m) : a(n, vector<K>(m, 0)) {}
  Matrix(size_t n) : Matrix(n, n) {}
  Matrix(vector<vector<K>> a) : a(a) {}
  size_t height() const { return a.size(); }
  size_t width() const { return a[0].size(); }
  inline const vector<K> &operator[](size_t k) const { return a[k]; }
  inline vector<K> &operator[](size_t k) { return a[k]; }

  static Matrix I(size_t n) {
    Matrix mat(n);
    for (int i = 0; i < n; i++) mat[i][i] = 1;
    return mat;
  }
  Matrix &operator+=(const Matrix &b) {
    size_t n = height(), m = width();
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++) (*this)[i][j] += b[i][j];
    return (*this);
  }
  Matrix &operator-=(const Matrix &b) {
    size_t n = height(), m = width();
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++) (*this)[i][j] -= b[i][j];
    return (*this);
  }
  Matrix &operator*=(const Matrix &b) {
    size_t n = height(), m = width(), l = b.width();
    assert(m == b.height());
    vector<vector<K>> c(n, vector<K>(l, 0));
    for (int i = 0; i < n; i++)
      for (int j = 0; j < l; j++)
        for (int k = 0; k < m; k++) c[i][j] += (*this)[i][k] * b[k][j];
    a.swap(c);
    return (*this);
  }
  Matrix operator+(const Matrix &b) const { return (Matrix(*this) += b); }
  Matrix operator-(const Matrix &b) const { return (Matrix(*this) -= b); }
  Matrix operator*(const Matrix &b) const { return (Matrix(*this) *= b); }
  Matrix pow(uint64_t e) const {
    Matrix ret = I(height());
    for (Matrix base(a); e; e >>= 1, base *= base)
      if (e & 1) ret *= base;
    return ret;
  }
  vector<K> operator*(const vector<K> &v) {
    size_t n = height(), m = width();
    assert(m == v.size());
    vector<K> ret(n);
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++) ret[i] += (*this)[i][j] * v[j];
    return ret;
  }
  bool operator==(const BitMatrix &b) const { return a == b.a; }
  template <typename T>
  using ET = enable_if<is_floating_point<T>::value>;
  template <typename T>
  using EF = enable_if<!is_floating_point<T>::value>;
  template <typename T, typename ET<T>::type * = nullptr>
  static bool is_zero(T x) {
    return abs(x) < 1e-8;
  }
  template <typename T, typename EF<T>::type * = nullptr>
  static bool is_zero(T x) {
    return x == T(0);
  }
  template <typename T, typename ET<T>::type * = nullptr>
  static bool compare(T x, T y) {
    return abs(x) < abs(y);
  }
  template <typename T, typename EF<T>::type * = nullptr>
  static bool compare(T x, T y) {
    (void)x;
    return y != T(0);
  }
  // O(nm(m+l))
  static pair<Matrix, Matrix> Gauss_Jordan(const Matrix &a, const Matrix &b) {
    size_t n = a.height(), m = a.width(), l = b.width();
    Matrix c(n, m + l);
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++) c[i][j] = a[i][j];
    for (int i = 0; i < n; i++)
      for (int j = 0; j < l; j++) c[i][j + m] = b[i][j];
    int d = 0;
    for (int j = 0; j < m; j++) {
      int p = d;
      for (int i = d + 1; i < n; i++)
        if (compare(c[p][j], c[i][j])) p = i;
      if (is_zero(c[p][j])) continue;
      swap(c[p], c[d]);
      K invc = K(1) / c[d][j];
      for (int k = j; k < m + l; k++) c[d][k] *= invc;
      for (int i = 0; i < n; i++) {
        if (i == d) continue;
        for (int k = m + l - 1; k >= j; k--) c[i][k] -= c[i][j] * c[d][k];
      }
      d++;
    }
    Matrix reta(n, m), retb(n, l);
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++) reta[i][j] = c[i][j];
    for (int i = 0; i < n; i++)
      for (int j = 0; j < l; j++) retb[i][j] = c[i][j + m];
    return make_pair(reta, retb);
  }
  // O(nm^2)
  static pair<vector<K>, vector<vector<K>>> linear_equations(
      const Matrix &a, const vector<K> &b) {
    int n = a.height(), m = a.width();
    Matrix B(n, 1);
    for (int i = 0; i < n; i++) B[i][0] = b[i];
    auto p = Gauss_Jordan(a, B);
    vector<int> jdx(n, -1), idx(m, -1);
    for (int i = 0, j; i < n; i++) {
      for (j = 0; j < m; j++) {
        if (!is_zero(p.first[i][j])) {
          jdx[i] = j, idx[j] = i;
          break;
        }
      }
      if (j == m && !is_zero(p.second[i][0]))
        return make_pair(vector<K>(), vector<vector<K>>());  // no solutions
    }
    vector<K> c(m);
    vector<vector<K>> d;
    for (int j = 0; j < m; j++) {
      if (idx[j] != -1)
        c[j] = p.second[idx[j]][0];
      else {
        vector<K> v(m);
        v[j] = 1;
        for (int i = 0; i < n; i++)
          if (jdx[i] != -1) v[jdx[i]] = -p.first[i][j];
        d.push_back(v);
      }
    }
    return make_pair(c, d);
  }
  // O(n^3)
  K det() const {
    size_t n = height();
    Matrix A(a);
    K ret(1);
    for (int i = 0; i < n; i++) {
      int p = i;
      for (int j = i + 1; j < n; j++)
        if (compare(A[i][p], A[i][j])) p = j;
      if (is_zero(A[i][p])) return 0;
      if (p != i) ret = -ret;
      swap(A[p], A[i]);
      ret *= A[i][i];
      K inva = K(1) / A[i][i];
      for (int j = i + 1; j < n; j++) {
        for (int k = n - 1; k >= i; k--) A[j][k] -= inva * A[j][i] * A[i][k];
      }
    }
    return ret;
  }
};
