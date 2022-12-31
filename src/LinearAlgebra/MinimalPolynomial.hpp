#pragma once
#include <bits/stdc++.h>
#include "src/Math/berlekamp_massey.hpp"
// c s.t. (c[d] * M^d + c[d-1] * M^(d-1)  + ... + c[1] * M + c[0]) * b = 0
template <class Mat, class Vec> class MinimalPolynomial {
 using mod_t= std::remove_reference_t<decltype((Vec{1})[0])>;
 static const inline mod_t ZERO= 0;
 std::vector<mod_t> poly, rev;
 std::vector<Vec> bs;
 std::size_t dg, n;
 static inline int deg(const std::vector<mod_t> &p) {
  for (int d= p.size() - 1;; d--)
   if (d < 0 || p[d] != ZERO) return d;
 }
 static inline std::vector<mod_t> bostan_mori_msb(const std::vector<mod_t> &q, std::uint64_t k) {
  int d= deg(q);
  assert(d >= 0), assert(q[0] != ZERO);
  std::vector<mod_t> ret(std::max(d, 1));
  if (k == 0) return ret.back()= mod_t(1), ret;
  std::vector<mod_t> v(d + 1);
  for (int i= 0; i <= d; i+= 2)
   for (int j= 0; j <= d; j+= 2) v[(i + j) >> 1]+= q[i] * q[j];
  for (int i= 1; i <= d; i+= 2)
   for (int j= 1; j <= d; j+= 2) v[(i + j) >> 1]-= q[i] * q[j];
  auto w= bostan_mori_msb(v, k >> 1);
  for (int i= 2 * d - 1 - (k & 1); i >= d; i-= 2)
   for (int j= 0; j <= d; j+= 2) ret[i - d]+= q[j] * w[(i - j) >> 1];
  for (int i= 2 * d - 1 - !(k & 1); i >= d; i-= 2)
   for (int j= 1; j <= d; j+= 2) ret[i - d]-= q[j] * w[(i - j) >> 1];
  return ret;
 }
 std::vector<mod_t> x_pow_mod(std::uint64_t k) const {
  assert(k >= n);
  std::vector<mod_t> ret(n), u= bostan_mori_msb(rev, k - n + dg);
  for (int i= dg; i--;)
   for (int j= i + 1; j--;) ret[n - 1 - i]+= u[j] * rev[i - j];
  return ret;
 }
public:
 MinimalPolynomial(const Mat &M, Vec b): n(M.size()) {
  std::size_t i, j;
  assert(n == b.size());
  std::vector<mod_t> a(n), v;
  for (auto &x: a) x= get_rand(1, mod_t::mod() - 1);
  mod_t tmp;
  for (i= (n + 1) << 1; i--; v.push_back(tmp)) {
   if (i > n) bs.emplace_back(b);
   for (tmp= 0, j= n; j--;) tmp+= a[j] * b[j];
   if (i) b= M * b;
  }
  rev= berlekamp_massey(v);
  for (auto &x: rev) x= -x;
  rev.insert(rev.begin(), 1), poly= rev;
  rev.erase(rev.begin() + (dg= deg(rev)) + 1, rev.end());
  std::reverse(poly.begin(), poly.end());
 }
 static std::uint64_t get_rand(std::uint64_t l, std::uint64_t r) {
  static std::mt19937_64 gen(std::random_device{}());
  return std::uniform_int_distribution<std::uint64_t>(l, r)(gen);
 }
 Vec pow(std::uint64_t k) const {  // M^k * b
  if (k < n) return bs[k];
  auto r= x_pow_mod(k);
  Vec ret= bs[0];
  for (auto &x: ret) x*= r[0];
  for (int i= 1, e= r.size(), j; i < e; i++)
   for (j= n; j--;) ret[j]+= r[i] * bs[i][j];
  return ret;
 }
 const mod_t operator[](std::size_t k) const { return poly[k]; }
 const auto begin() const { return poly.begin(); }
 const auto end() const { return poly.end(); }
 const std::size_t size() const { return dg + 1; }
};