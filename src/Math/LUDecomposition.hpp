#pragma once
#include <bits/stdc++.h>
/**
 * @title LU分解
 * @category 数学
 * bool型の場合のkernel関数 未verify
 */

// BEGIN CUT HERE

template <class K, std::size_t MAX_ROWS = (1 << 12),
          std::size_t MAX_COLS = MAX_ROWS>
class LUDecomposition {
  using Mat = std::vector<std::vector<K>>;
  Mat dat;
  std::vector<std::size_t> perm, piv;
  bool sgn;
  template <class T>
  inline static constexpr bool IFPV = std::is_floating_point_v<T>;
  template <class T, typename std::enable_if_t<IFPV<T>> * = nullptr>
  static bool is_zero(T x) {
    return std::abs(x) < 1e-8;
  }
  template <class T, typename std::enable_if_t<!IFPV<T>> * = nullptr>
  static bool is_zero(T x) {
    return x == T(0);
  }

 public:
  LUDecomposition(const Mat &A) : dat(A), perm(A.size()), sgn(false) {
    std::size_t rows = A.size(), cols = A[0].size();
    std::iota(perm.begin(), perm.end(), 0);
    for (std::size_t c = 0; c != cols && piv.size() != rows; c++) {
      auto pos = piv.size();
      if constexpr (IFPV<K>) {
        for (std::size_t r = piv.size() + 1; r < rows; r++)
          if (std::abs(dat[pos][c]) < std::abs(dat[r][c])) pos = r;
      } else if (is_zero(dat[pos][c])) {
        for (std::size_t r = piv.size() + 1; r < rows; r++)
          if (!is_zero(dat[r][c])) pos = r, r = rows;
      }
      if (is_zero(dat[pos][c])) continue;
      if (pos != piv.size())
        sgn = !sgn, std::swap(perm[pos], perm[piv.size()]),
        std::swap(dat[pos], dat[piv.size()]);
      for (std::size_t r = piv.size() + 1; r != rows; r++) {
        auto m = dat[r][c] / dat[pos][c];
        dat[r][c] = 0, dat[r][piv.size()] = m;
        for (std::size_t i = c + 1; i != cols; i++)
          dat[r][i] -= dat[piv.size()][i] * m;
      }
      piv.emplace_back(c);
    }
  }
  std::size_t rank() const { return piv.size(); }
  bool isregular() const {
    return rank() == dat.size() && rank() == dat[0].size();
  }
  K det() const {
    assert(dat.size() == dat[0].size());
    K d = sgn ? -1 : 1;
    for (std::size_t i = 0; i != dat.size(); i++) d *= dat[i][i];
    return d;
  }
  std::vector<std::vector<K>> kernel() const {
    std::size_t cols = dat[0].size();
    std::vector<std::vector<K>> ker(cols - rank(), std::vector<K>(cols));
    for (std::size_t c = 0, i = 0; c != cols; c++) {
      if (i != piv.size() && piv[i] == c) {
        i++;
        continue;
      }
      ker[c - i][c] = 1;
      for (std::size_t r = 0; r != i; r++) ker[c - i][r] = -dat[r][c];
      for (std::size_t j = i; j--;) {
        auto x = ker[c - i][j] / dat[j][piv[j]];
        ker[c - i][j] = 0, ker[c - i][piv[j]] = x;
        for (std::size_t r = 0; r != j; r++)
          ker[c - i][r] -= dat[r][piv[j]] * x;
      }
    }
    return ker;
  }
  std::vector<K> linear_equations(const std::vector<K> &b) const {
    std::size_t rows = dat.size(), cols = dat[0].size();
    assert(rows <= b.size());
    std::vector<K> y(rows), x(cols);
    for (std::size_t c = 0; c != rows; c++) {
      y[c] += b[perm[c]];
      if (c < cols)
        for (std::size_t r = c + 1; r != rows; r++) y[r] -= y[c] * dat[r][c];
    }
    for (std::size_t i = rank(); i != rows; i++)
      if (!is_zero(y[i])) return {};  // no solution
    for (std::size_t i = rank(); i--;) {
      x[piv[i]] = y[i] / dat[i][piv[i]];
      for (std::size_t r = 0; r != i; r++) y[r] -= x[piv[i]] * dat[r][piv[i]];
    }
    return x;
  }
  Mat inverse_matrix() const {
    if (!isregular()) return {};  // no solution
    assert(dat.size() == dat[0].size());
    std::vector<K> b(dat.size());
    Mat ret;
    for (std::size_t i = 0; i < dat.size(); b[i++] = 0)
      b[i] = 1, ret.emplace_back(linear_equations(b));
    for (std::size_t i = 0; i < dat.size(); i++)
      for (std::size_t j = 0; j < i; j++) std::swap(ret[i][j], ret[j][i]);
    return ret;
  }
};

template <std::size_t MAX_ROWS, std::size_t MAX_COLS>
class LUDecomposition<bool, MAX_ROWS, MAX_COLS> {
 public:
  using Mat = std::vector<std::vector<bool>>;
  std::size_t rows, cols;
  std::vector<std::bitset<MAX_ROWS>> tdat, tdat2;
  std::vector<std::size_t> perm, piv;

 public:
  LUDecomposition(const Mat &A)
      : rows(A.size()), cols(A[0].size()), tdat(cols), tdat2(cols), perm(rows) {
    std::vector<std::bitset<MAX_COLS>> dat(rows);
    for (std::size_t i = 0; i < rows; i++)
      for (std::size_t j = 0; j < cols; j++) dat[i][j] = A[i][j];
    std::iota(perm.begin(), perm.end(), 0);
    std::bitset<MAX_COLS> mask;
    for (std::size_t c = 1; c < cols; c++) mask.set(c);
    for (std::size_t c = 0; c != cols && piv.size() != rows; mask.reset(++c)) {
      auto pos = piv.size();
      for (std::size_t r = piv.size(); r < rows; r++)
        if (bool(dat[r][c])) pos = r, r = rows;
      if (!bool(dat[pos][c])) continue;
      if (pos != piv.size())
        std::swap(perm[pos], perm[piv.size()]),
            std::swap(dat[pos], dat[piv.size()]);
      for (std::size_t r = piv.size() + 1; r != rows; r++) {
        auto m = bool(dat[r][c]);
        dat[r][c] = 0, dat[r][piv.size()] = m;
        if (m) dat[r] ^= dat[piv.size()] & mask;
      }
      piv.emplace_back(c);
    }
    for (std::size_t j = 0; j < cols; j++)
      for (std::size_t i = j + 1; i < rows; i++) tdat[j][i] = dat[i][j];
    for (std::size_t c = 0, i = 0; c < cols; c++) {
      for (std::size_t r = 0; r < i; r++) tdat2[c][r] = dat[r][c];
      if (i != piv.size() && piv[i] == c) i++;
    }
  }
  std::size_t rank() const { return piv.size(); }
  bool isregular() const { return rank() == rows && rank() == cols; }
  bool det() const { return isregular(); }
  std::vector<std::vector<bool>> kernel() const {
    std::vector<std::bitset<MAX_COLS>> ker(cols - rank());
    for (std::size_t c = 0, i = 0; c != cols; c++) {
      if (i != piv.size() && piv[i] == c) {
        i++;
        continue;
      }
      ker[c - i] = tdat2[c], ker[c - i][c] = 1;
      for (std::size_t j = i; j--;) {
        auto x = bool(ker[c - i][j]);
        ker[c - i][j] = 0, ker[c - i][piv[j]] = x;
        if (x) ker[c - i] ^= tdat2[piv[j]];
      }
    }
    std::vector<std::vector<bool>> ret(cols - rank(), std::vector<bool>(cols));
    for (std::size_t i = cols - rank(); i--;)
      for (std::size_t j = 0; j < cols; j++) ret[i][j] = ker[i][j];
    return ret;
  }
  std::vector<bool> linear_equations(const std::vector<bool> &b) const {
    assert(rows <= b.size());
    std::bitset<MAX_COLS> y;
    std::vector<bool> x(cols);
    for (std::size_t c = 0; c != rows; c++) {
      if (b[perm[c]]) y[c].flip();
      if (c < cols && bool(y[c])) y ^= tdat[c];
    }
    for (std::size_t i = rank(); i != rows; i++)
      if (bool(y[i])) return {};  // no solution
    for (std::size_t i = rank(); i--;) {
      x[piv[i]] = y[i];
      if (x[piv[i]]) y ^= tdat2[piv[i]];
    }
    return x;
  }
  Mat inverse_matrix() const {
    if (!isregular()) return {};  // no solution
    std::vector<bool> b(rows);
    Mat ret;
    for (std::size_t i = 0; i < rows; b[i++] = 0)
      b[i] = 1, ret.emplace_back(linear_equations(b));
    for (std::size_t i = 0; i < rows; i++)
      for (std::size_t j = 0; j < i; j++) std::swap(ret[i][j], ret[j][i]);
    return ret;
  }
};