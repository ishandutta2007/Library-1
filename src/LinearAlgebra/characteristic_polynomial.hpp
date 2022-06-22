#pragma once
#include <bits/stdc++.h>
/**
 * @title 行列の特性多項式
 * @category 線形代数
 * O(N^3)
 */

// BEGIN CUT HERE

template <class K>
std::vector<std::vector<K>> hessenberg(const std::vector<std::vector<K>> &a) {
  std::size_t n = a.size();
  auto ret = a;
  for (std::size_t j = 0; j + 2 < n; ++j) {
    for (std::size_t i = j + 1; i < n; ++i)
      if (ret[i][j] != 0) {
        std::swap(ret[j + 1], ret[i]);
        for (std::size_t r = 0; r < n; ++r) std::swap(ret[r][j + 1], ret[r][i]);
        break;
      }
    if (ret[j + 1][j] != 0)
      for (std::size_t i = j + 2; i < n; ++i) {
        auto m = ret[i][j] / ret[j + 1][j];
        for (std::size_t c = j; c < n; ++c) ret[i][c] -= m * ret[j + 1][c];
        for (std::size_t r = 0; r < n; ++r) ret[r][j + 1] += m * ret[r][i];
      }
  }
  return ret;
}

template <class K>
std::vector<K> characteristic_polynomial(const std::vector<std::vector<K>> &a) {
  std::size_t n = a.size();
  auto b = hessenberg(a);
  for (std::size_t i = 0; i < n; i++)
    for (std::size_t j = 0; j < n; j++) b[i][j] = -b[i][j];
  std::vector<std::vector<K>> fss(n + 1);
  fss[0] = {1};
  for (std::size_t i = 0; i < n; ++i) {
    fss[i + 1].assign(i + 2, 0);
    for (std::size_t k = 0; k <= i; ++k) fss[i + 1][k + 1] = fss[i][k];
    for (std::size_t k = 0; k <= i; ++k) fss[i + 1][k] += b[i][i] * fss[i][k];
    K prod = 1;
    for (std::size_t j = i; j--;) {
      prod *= -b[j + 1][j];
      const K s = prod * b[j][i];
      for (std::size_t k = 0; k <= j; ++k) fss[i + 1][k] += s * fss[j][k];
    }
  }
  return fss.back();
}
