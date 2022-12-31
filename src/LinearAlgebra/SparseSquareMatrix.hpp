#pragma once
#include <bits/stdc++.h>
#include "src/LinearAlgebra/MinimalPolynomial.hpp"
template <class mod_t> struct SparseSquareMatrix {
 SparseSquareMatrix(std::size_t n_): n(n_) {}
 void add_component(std::size_t i, std::size_t j, mod_t val) { dat.emplace_back(i, j, val); }
 std::vector<mod_t> operator*(const std::vector<mod_t> &vec) const {
  std::vector<mod_t> ret(n);
  assert(vec.size() == n);
  for (const auto &[i, j, val]: dat) ret[i]+= val * vec[j];
  return ret;
 }
 auto begin() { return dat.begin(); }
 auto end() { return dat.end(); }
 std::size_t size() const { return n; }
 mod_t det() const {
  const std::uint64_t MOD= mod_t::mod();
  using MinPoly= MinimalPolynomial<SparseSquareMatrix, std::vector<mod_t>>;
  SparseSquareMatrix M(*this);
  std::vector<mod_t> d(n), b(n);
  for (auto &x: b) x= MinPoly::get_rand(1, MOD - 1);
  for (auto &x: d) x= MinPoly::get_rand(1, MOD - 1);
  for (auto &[i, j, val]: M) val*= d[j];
  mod_t ret= MinPoly(M, b)[0], tmp= 1;
  for (const auto &x: d) tmp*= x;
  if (n & 1) ret= -ret;
  return ret / tmp;
 }
private:
 std::size_t n;
 std::vector<std::tuple<std::size_t, std::size_t, mod_t>> dat;
};
