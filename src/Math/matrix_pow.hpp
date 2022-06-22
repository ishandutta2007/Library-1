#pragma once
#include <bits/stdc++.h>
#include "src/Math/minimal_polynomial.hpp"
#include "src/Math/x_pow_mod.hpp"
/**
 * @title 行列累乗
 * @category 数学
 * @see https://qiita.com/ryuhe1/items/c18ddbb834eed724a42b
 * @see https://yukicoder.me/wiki/black_box_linear_algebra
 * @brief O(N*S(N) + N^2 log k) S(N): M*bの計算量
 */

// BEGIN CUT HERE

// M^k * b
template <class Mat, class Vec>
Vec matrix_pow(const Mat &M, Vec b, std::uint64_t k) {
  std::size_t n = M.size();
  assert(n == b.size());
  auto r = x_pow_mod(minimal_polynomial(M, b), k);
  Vec ret = b;
  for (auto &x : ret) x *= r[0];
  for (int i = 1, e = r.size(), j; i < e; i++)
    for (b = M * b, j = n; j--;) ret[j] += r[i] * b[j];
  return ret;
}