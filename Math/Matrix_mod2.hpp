/**
 * @title 行列(mod2)
 * @category 数学
 * @brief Gauss_Jordan(A,B) 拡大係数行列に対するガウスジョルダン法
 * @brief linear_equations 返り値 {解のうちの一つ,解空間の基底ベクトル}
 */
// verify用:
// https://atcoder.jp/contests/bitflyer2018-final-open/tasks/bitflyer2018_final_d

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

struct Matrix_mod2 {
 private:
  vector<vector<short>> a;

 public:
  Matrix_mod2() {}
  Matrix_mod2(size_t n, size_t m) : a(n, vector<short>(m, 0)) {}
  Matrix_mod2(size_t n) : Matrix_mod2(n, n) {}
  inline const vector<short> &operator[](size_t k) const { return a[k]; }
  inline vector<short> &operator[](size_t k) { return a[k]; }
  size_t height() const { return a.size(); }
  size_t width() const { return a[0].size(); }

  static Matrix_mod2 I(size_t n) {
    Matrix_mod2 mat(n);
    for (int i = 0; i < n; i++) mat[i][i] = 1;
    return mat;
  }
  Matrix_mod2 operator+(const Matrix_mod2 &b) const {
    size_t n = height(), m = width();
    Matrix_mod2 c(n, m);
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++) c[i][j] = (*this)[i][j] ^ b[i][j];
    return c;
  }
  Matrix_mod2 operator*(const Matrix_mod2 &b) const {
    if (width() <= 64) return mul<64>(b);
    if (width() <= 2600) return mul<2600>(b);
    return mul<100010>(b);
  }
  Matrix_mod2 &operator+=(const Matrix_mod2 &b) { return *this = (*this) + b; }
  Matrix_mod2 &operator*=(const Matrix_mod2 &b) { return *this = (*this) * b; }
  Matrix_mod2 pow(uint64_t e) const {
    Matrix_mod2 ret = I(height());
    for (Matrix_mod2 base = *this; e; e >>= 1, base *= base)
      if (e & 1) ret *= base;
    return ret;
  }
  bool operator==(const Matrix_mod2 &b) const { return a == b.a; }

  static pair<Matrix_mod2, Matrix_mod2> Gauss_Jordan(const Matrix_mod2 &a,
                                                     const Matrix_mod2 &b) {
    if (a.width() + b.width() <= 64) return gauss_jordan_content<64>(a, b);
    if (a.width() + b.width() <= 2600) return gauss_jordan_content<2600>(a, b);
    return gauss_jordan_content<100010>(a, b);
  }
  static pair<vector<int>, vector<vector<int>>> linear_equations(
      const Matrix_mod2 &a, const vector<int> &b) {
    int n = a.height(), m = a.width();
    Matrix_mod2 B(n, 1);
    for (int i = 0; i < n; i++) B[i][0] = b[i];
    auto p = Gauss_Jordan(a, B);
    vector<int> jdx(n, -1), idx(m, -1);
    for (int i = 0, j; i < n; i++) {
      for (j = 0; j < m; j++) {
        if (p.first[i][j]) {
          jdx[i] = j, idx[j] = i;
          break;
        }
      }
      if (j == m && p.second[i][0])
        return make_pair(vector<int>(), vector<vector<int>>());  // no solutions
    }
    vector<int> c(m);
    vector<vector<int>> d;
    for (int j = 0; j < m; j++) {
      if (idx[j] != -1)
        c[j] = p.second[idx[j]][0];
      else {
        vector<int> v(m);
        v[j] = 1;
        for (int i = 0; i < n; i++)
          if (jdx[i] != -1) v[jdx[i]] = p.first[i][j];
        d.push_back(v);
      }
    }
    return make_pair(c, d);
  }
  int rank() const {
    int n = height(), m = width();
    Matrix_mod2 b(n, 0);
    Matrix_mod2 p = Gauss_Jordan(*this, b).first;
    for (int i = 0, j; i < n; i++) {
      for (j = 0; j < m; j++)
        if (p[i][j] != 0) break;
      if (j == m) return i;
    }
    return n;
  }

 private:
  template <size_t SIZE>
  Matrix_mod2 mul(const Matrix_mod2 &b) const {
    size_t n = height(), m = width(), l = b.width();
    assert(m == b.height());
    vector<bitset<SIZE>> tb(l);
    for (int i = 0; i < l; ++i)
      for (int j = 0; j < m; ++j) tb[i][j] = b[j][i];
    Matrix_mod2 c(n, l);
    for (int i = 0; i < n; i++) {
      bitset<SIZE> abit;
      for (int k = 0; k < m; k++) abit[k] = (*this)[i][k];
      for (int j = 0; j < l; j++) c[i][j] = ((abit & tb[j]).count() & 1);
    }
    return c;
  }

  template <size_t SIZE>
  static pair<Matrix_mod2, Matrix_mod2> gauss_jordan_content(
      const Matrix_mod2 &a, const Matrix_mod2 &b) {
    size_t n = a.height(), m = a.width(), l = b.width();
    vector<bitset<SIZE>> c(n);
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++) c[i][j] = a[i][j];
    for (int i = 0; i < n; i++)
      for (int j = 0; j < l; j++) c[i][j + m] = b[i][j];
    for (int j = 0, d = 0; j < m && d < n; j++) {
      int p = d;
      for (int i = d + 1; i < n; i++)
        if (c[i][j]) p = i;
      if (!c[p][j]) continue;
      swap(c[p], c[d]);
      for (int i = 0; i < n; i++)
        if (i != d && c[i][j]) c[i] ^= c[d];
      d++;
    }
    Matrix_mod2 reta(n, m), retb(n, l);
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++) reta[i][j] = c[i][j];
    for (int i = 0; i < n; i++)
      for (int j = 0; j < l; j++) retb[i][j] = c[i][j + m];
    return make_pair(reta, retb);
  }
};
