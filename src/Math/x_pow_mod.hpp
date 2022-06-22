#pragma once
#include <bits/stdc++.h>
/**
 * @title $x^k \;\mathrm{mod}g(x)$
 * @category 数学
 * @see https://qiita.com/ryuhe1/items/c18ddbb834eed724a42b
 * @brief 多項式乗算はO(N^2)で実装
 */

// BEGIN CUT HERE

template <class K>
int deg(const std::vector<K> &p) {
  const K ZERO = 0;
  for (int n = p.size() - 1;; n--)
    if (n < 0 || p[n] != ZERO) return n;
}
template <class K>
std::vector<K> bostan_mori_msb(const std::vector<K> &q, std::uint64_t k) {
  const int d = deg(q);
  assert(d >= 0), assert(q[0] != K(0));
  std::vector<K> ret(std::max(d, 1));
  if (k == 0) return ret.back() = K(1) / q[0], ret;
  std::vector<K> v(d + 1);
  for (int i = 0; i <= d; i += 2)
    for (int j = 0; j <= d; j += 2) v[(i + j) >> 1] += q[i] * q[j];
  for (int i = 1; i <= d; i += 2)
    for (int j = 1; j <= d; j += 2) v[(i + j) >> 1] -= q[i] * q[j];
  auto w = bostan_mori_msb(v, k >> 1);
  for (int i = 2 * d - 1 - (k & 1); i >= d; i -= 2)
    for (int j = 0; j <= d; j += 2) ret[i - d] += q[j] * w[(i - j) >> 1];
  for (int i = 2 * d - 1 - !(k & 1); i >= d; i -= 2)
    for (int j = 1; j <= d; j += 2) ret[i - d] -= q[j] * w[(i - j) >> 1];
  return ret;
}
// x^k mod g(x)
template <class K>
std::vector<K> x_pow_mod(const std::vector<K> &g, std::uint64_t k) {
  const int d = deg(g);
  assert(d >= 0);
  std::vector<K> q(g.begin(), g.begin() + d + 1), ret(d);
  std::reverse(q.begin(), q.end());
  auto u = bostan_mori_msb(q, k);
  for (int i = d; i--;)
    for (int j = i + 1; j--;) ret[d - 1 - i] += u[j] * q[i - j];
  return ret;
}