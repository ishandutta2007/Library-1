#pragma once
#include <bits/stdc++.h>
/**
 * @title ガウスの消去法
 * @category 数学
 * linear_equation(A,b) 返り値 {解のうちの一つ,解空間の基底ベクトル}
 */

// BEGIN CUT HERE

class GaussianElimination {
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
  template <class T, typename std::enable_if_t<IFPV<T>> * = nullptr>
  static bool compare(T x, T y) {
    return std::abs(x) < std::abs(y);
  }
  template <class T, typename std::enable_if_t<!IFPV<T>> * = nullptr>
  static bool compare(T, T y) {
    return y != T(0);
  }
  template <class LHS, class RHS>
  static void subst(LHS &lhs, const RHS &rhs, int n, int m) {
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++) lhs[i][j] = rhs[i][j];
  }
  template <int M>
  static int row_reduction(std::vector<std::bitset<M>> &a, int lim = 1 << 30) {
    int n = a.size(), rank = 0, j, p;
    for (lim = std::min(lim, M), j = 0, p = rank; j < lim; j++, p = rank) {
      while (p < n - 1 && !a[p][j]) p++;
      if (!a[p][j]) continue;
      std::swap(a[p], a[rank]);
      for (int i = 0; i < n; i++)
        if (i != rank && a[i][j]) a[i] ^= a[rank];
      if (++rank == n) break;
    }
    return rank;
  }

 public:
  template <class K>
  static auto row_reduction(std::vector<std::vector<K>> &a, int lim = 1 << 30) {
    int n = a.size(), m = a[0].size(), rank = 0, j, p;
    K det = K(1), invc;
    for (lim = std::min(lim, m), j = 0, p = rank; j < lim; j++, p = rank) {
      for (int i = rank + 1; i < n; i++)
        if (compare(a[p][j], a[i][j])) p = i;
      if (is_zero(a[p][j])) continue;
      if (p != rank) std::swap(a[p], a[rank]), det = -det;
      invc = K(1) / a[rank][j], det *= a[rank][j];
      for (int k = j; k < m; k++) a[rank][k] *= invc;
      for (int i = 0; i < n; i++)
        if (i != rank && !is_zero(a[i][j]))
          for (int k = m - 1; k >= j; k--) a[i][k] -= a[i][j] * a[rank][k];
      if (++rank == n) break;
    }
    return std::make_pair(rank, rank == n ? det : K(0));
  }
  static auto row_reduction(std::vector<std::vector<bool>> &a,
                            int lim = 1 << 30) {
    int n = a.size(), m = a[0].size(), rank;
    if (m < 1024) {
      std::vector<std::bitset<1024>> b(n);
      subst(b, a, n, m), rank = row_reduction<1024>(b, lim), subst(a, b, n, m);
    } else {
      std::vector<std::bitset<4096>> b(n);
      subst(b, a, n, m), rank = row_reduction<4096>(b, lim), subst(a, b, n, m);
    }
    return std::make_pair(rank, rank == n);
  }
  template <class K>
  static std::pair<std::vector<K>, std::vector<std::vector<K>>>
  linear_equations(std::vector<std::vector<K>> a, const std::vector<K> &b) {
    int n = a.size(), m = a[0].size();
    for (int i = 0; i < n; i++) a[i].emplace_back(b[i]);
    int rank = row_reduction(a, m).first;
    for (int i = rank; i < n; ++i)
      if (!is_zero(K(a[i][m]))) return {{}, {}};
    std::vector<K> c(m, K(0));
    std::vector<int> piv(m, -1);
    for (int i = 0, j = 0; i < rank; i++) {
      while (is_zero(K(a[i][j]))) j++;
      c[j] = a[i][m], piv[j] = i;
    }
    std::vector<std::vector<K>> d;
    for (int j = 0; j < m; ++j) {
      if (piv[j] != -1) continue;
      std::vector<K> x(m, K(0));
      x[j] = K(-1);
      for (int k = 0; k < j; ++k)
        if (piv[k] != -1) x[k] = a[piv[k]][j];
      d.emplace_back(x);
    }
    return {c, d};
  }
};