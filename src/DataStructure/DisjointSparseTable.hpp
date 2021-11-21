#pragma once
#include <bits/stdc++.h>
/**
 * @title Disjoint-Sparse-Table
 * @category データ構造
 * @brief fは結合則をみたす二項演算
 * @brief 構築 O(n log n)
 * @brief query O(1)
 */

// BEGIN CUT HERE

template <class T>
struct DisjointSparseTable {
  std::vector<std::vector<T>> ys;
  using F = std::function<T(T, T)>;
  const F f;
  DisjointSparseTable(std::vector<T> xs, F f_) : f(f_) {
    int n = 1;
    while (n <= xs.size()) n *= 2;
    xs.resize(n);
    ys.emplace_back(xs);
    for (int h = 1;; ++h) {
      int range = (2 << h), half = range / 2;
      if (range > n) break;
      ys.emplace_back(xs);
      for (int i = half; i < n; i += range) {
        for (int j = i - 2; j >= i - half; --j)
          ys[h][j] = f(ys[h][j], ys[h][j + 1]);
        for (int j = i + 1; j < std::min(n, i + half); ++j)
          ys[h][j] = f(ys[h][j - 1], ys[h][j]);
      }
    }
  }
  T fold(int i, int j) {  // [i, j)
    if (i == --j) return ys[0][i];
    int h = sizeof(int) * __CHAR_BIT__ - 1 - __builtin_clz(i ^ j);
    return f(ys[h][i], ys[h][j]);
  }
};