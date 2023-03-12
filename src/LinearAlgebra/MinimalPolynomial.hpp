#pragma once
#include <bits/stdc++.h>
#include "src/Math/berlekamp_massey.hpp"
#include "src/LinearAlgebra/Vector.hpp"
#include "src/Math/ModInt.hpp"
// c s.t. (c[d] * M^d + c[d-1] * M^(d-1)  + ... + c[1] * M + c[0]) * b = 0
template <class mod_t, template <class> class Mat> class MinimalPolynomial {
 std::vector<mod_t> poly, rev;
 size_t dg, n;
 std::vector<Vector<mod_t>> bs;
 static inline int deg(const std::vector<mod_t> &p) {
  for (int d= p.size() - 1;; d--)
   if (d < 0 || p[d] != mod_t()) return d;
 }
 static inline std::vector<mod_t> bostan_mori_msb(const std::vector<mod_t> &q, uint64_t k) {
  int d= deg(q);
  assert(d >= 0), assert(q[0] != mod_t());
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
 std::vector<mod_t> x_pow_mod(uint64_t k) const {
  assert(k >= n);
  std::vector<mod_t> ret(n), u= bostan_mori_msb(rev, k - n + dg);
  for (int i= dg; i--;)
   for (int j= i + 1; j--;) ret[n - 1 - i]+= u[j] * rev[i - j];
  return ret;
 }
public:
 MinimalPolynomial(const Mat<mod_t> &M, Vector<mod_t> b): n(M.width()), bs(n) {
  static_assert(is_modint_v<mod_t>);
  assert(n == b.size()), assert(n == M.height());
  Vector<mod_t> a(n);
  for (auto &x: a) x= rng(1, mod_t::mod() - 1);
  std::vector<mod_t> v((n + 1) << 1);
  for (size_t i= v.size(), j= 0;; b= M * b) {
   if (j < n) bs[j]= b;
   if (v[j++]= (a * b).sum(); !(--i)) break;
  }
  rev= berlekamp_massey(v);
  for (auto &x: rev) x= -x;
  rev.insert(rev.begin(), 1), poly.assign(rev.rbegin(), rev.rend()), rev.erase(rev.begin() + (dg= deg(rev)) + 1, rev.end());
 }
 Vector<mod_t> pow(uint64_t k) const {  // M^k * b
  if (k < n) return bs[k];
  auto r= x_pow_mod(k);
  Vector<mod_t> ret= r[0] * bs[0];
  for (int i= r.size(); --i;) ret+= r[i] * bs[i];
  return ret;
 }
 const mod_t &operator[](size_t k) const { return poly[k]; }
 const auto begin() const { return poly.begin(); }
 const auto end() const { return poly.end(); }
 size_t degree() const { return dg; }
};
template <class mod_t, template <class> class Mat> mod_t det(const Mat<mod_t> &M) {
 size_t n= M.height();
 assert(n == M.width());
 Vector<mod_t> b(n);
 for (auto &x: b) x= rng(1, mod_t::mod() - 1);
 DiagonalMatrix<mod_t> D(n);
 for (auto &x: D) x= rng(1, mod_t::mod() - 1);
 mod_t ret= MinimalPolynomial(M * D, b)[0];
 if (n & 1) ret= -ret;
 return ret / D.det();
}