#pragma once
#include <bits/stdc++.h>
#include "src/Math/berlekamp_massey.hpp"
/**
 * @title 最小多項式と疎行列
 * @category 数学
 * @see https://yukicoder.me/wiki/black_box_linear_algebra
 */

// BEGIN CUT HERE

std::uint64_t get_rand(std::uint64_t l, std::uint64_t r) {
  static std::random_device rd;
  static std::mt19937_64 gen(rd());
  return std::uniform_int_distribution<std::uint64_t>(l, r)(gen);
}

// (c[d] * M^d + c[d-1] * M^(d-1)  + ... + c[1] * M + c[0]) * b = 0
// return c
template <class Mat, class Vec>
auto minimal_polynomial(const Mat &M, Vec b) {
  using mod_t = std::remove_reference_t<decltype(b[0])>;
  const std::uint64_t MOD = mod_t::modulo();
  std::size_t n = M.size(), i, j;
  assert(n == b.size());
  std::vector<mod_t> a(n), v;
  for (auto &x : a) x = get_rand(1, MOD - 1);
  mod_t tmp;
  for (i = (n + 1) << 1; i--; v.push_back(tmp)) {
    for (tmp = 0, j = n; j--;) tmp += a[j] * b[j];
    if (i) b = M * b;
  }
  auto ret = berlekamp_massey(v);
  for (auto &x : ret) x = -x;
  return std::reverse(ret.begin(), ret.end()), ret.push_back(1), ret;
}

template <class mod_t>
struct SparseSquareMatrix {
  SparseSquareMatrix(std::size_t n_) : n(n_) {}
  void add_component(std::size_t i, std::size_t j, mod_t val) {
    dat.emplace_back(i, j, val);
  }
  std::vector<mod_t> operator*(const std::vector<mod_t> &vec) const {
    std::vector<mod_t> ret(n);
    assert(vec.size() == n);
    for (const auto &[i, j, val] : dat) ret[i] += val * vec[j];
    return ret;
  }
  auto begin() { return dat.begin(); }
  auto end() { return dat.end(); }
  std::size_t size() const { return n; }
  mod_t det() const {
    const std::uint64_t MOD = mod_t::modulo();
    SparseSquareMatrix M(*this);
    std::vector<mod_t> d(n), b(n);
    for (auto &x : b) x = get_rand(1, MOD - 1);
    for (auto &x : d) x = get_rand(1, MOD - 1);
    for (auto &[i, j, val] : M) val *= d[j];
    mod_t ret = minimal_polynomial(M, b).front(), tmp = 1;
    for (const auto &x : d) tmp *= x;
    if (n & 1) ret = -ret;
    return ret / tmp;
  }

 private:
  std::size_t n;
  std::vector<std::tuple<std::size_t, std::size_t, mod_t>> dat;
};
