#pragma once
#include <bits/stdc++.h>
#include "src/Math/berlekamp_massey.hpp"
/**
 * @title 行列の最小多項式
 * @category 線形代数
 * @see https://yukicoder.me/wiki/black_box_linear_algebra
 * O(N*S(N))
 */

// BEGIN CUT HERE

// c s.t. (c[d] * M^d + c[d-1] * M^(d-1)  + ... + c[1] * M + c[0]) * b = 0
template <class Mat, class Vec>
class MinimalPolynomial {
  using mod_t = std::remove_reference_t<decltype((Vec{1})[0])>;
  static const inline mod_t ZERO = 0;
  std::vector<mod_t> poly, rev;
  std::vector<Vec> bs;
  int dg, n;
  std::vector<mod_t> bostan_mori_msb(const std::vector<mod_t> &q,
                                     std::uint64_t k) {
    int d = q.size() - 1;
    for (;; d--)
      if (d < 0 || q[d] != ZERO) break;
    assert(d >= 0), assert(q[0] != ZERO);
    std::vector<mod_t> ret(std::max(d, 1));
    if (k == 0) return ret.back() = mod_t(1) / q[0], ret;
    std::vector<mod_t> v(d + 1);
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
  std::vector<mod_t> x_pow_mod(std::uint64_t k) {
    std::vector<mod_t> ret(dg), u = bostan_mori_msb(rev, k);
    for (int i = dg; i--;)
      for (int j = i + 1; j--;) ret[dg - 1 - i] += u[j] * rev[i - j];
    return ret;
  }

 public:
  MinimalPolynomial(const Mat &M, Vec b) : n(M.size()) {
    std::size_t i, j;
    assert(n == b.size());
    std::vector<mod_t> a(n), v;
    for (auto &x : a) x = get_rand(1, mod_t::modulo() - 1);
    mod_t tmp;
    for (i = (n + 1) << 1; i--; v.push_back(tmp)) {
      if (i > n) bs.emplace_back(b);
      for (tmp = 0, j = n; j--;) tmp += a[j] * b[j];
      if (i) b = M * b;
    }
    rev = berlekamp_massey(v), dg = rev.size();
    for (auto &x : rev) x = -x;
    rev.insert(rev.begin(), 1), poly = rev;
    std::reverse(poly.begin(), poly.end());
    bs.erase(bs.begin() + dg + 1, bs.end());
  }
  static std::uint64_t get_rand(std::uint64_t l, std::uint64_t r) {
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    return std::uniform_int_distribution<std::uint64_t>(l, r)(gen);
  }
  Vec pow(std::uint64_t k) {  // M^k * b
    auto r = x_pow_mod(k);
    Vec ret = bs[0];
    for (auto &x : ret) x *= r[0];
    for (int i = 1, e = r.size(), j; i < e; i++)
      for (j = n; j--;) ret[j] += r[i] * bs[i][j];
    return ret;
  }
  const mod_t operator[](std::size_t k) const { return poly[k]; }
  const auto begin() const { return poly.begin(); }
  const auto end() const { return poly.end(); }
  const std::size_t size() const { return dg + 1; }
};