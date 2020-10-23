#pragma once
#include <bits/stdc++.h>
/**
 * @title Sparse-Table(区間min)
 * @category データ構造
 * @brief RmQ
 * @brief 構築 O(n log n)
 * @brief query O(1)
 */

// BEGIN CUT HERE

template <class T>
struct SparseTable {
  const std::vector<T> &x;
  std::vector<std::vector<int>> table;
  int argmin(int i, int j) { return x[i] < x[j] ? i : j; }
  SparseTable(const std::vector<T> &x) : x(x) {
    int logn = sizeof(int) * __CHAR_BIT__ - 1 - __builtin_clz(x.size());
    table.assign(logn + 1, std::vector<int>(x.size()));
    iota(table[0].begin(), table[0].end(), 0);
    for (int h = 0; h + 1 <= logn; ++h)
      for (int i = 0; i + (1 << h) < x.size(); ++i)
        table[h + 1][i] = argmin(table[h][i], table[h][i + (1 << h)]);
  }
  T range_min(int i, int j) {  // = min x[i,j)
    int h = sizeof(int) * __CHAR_BIT__ - 1 - __builtin_clz(j - i);  // = log2
    return x[argmin(table[h][i], table[h][j - (1 << h)])];
  }
};