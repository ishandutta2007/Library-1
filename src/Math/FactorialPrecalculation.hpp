#pragma once
#include <cassert>
#include <vector>
#include "src/Internal/modint_traits.hpp"
template <class mod_t> class FactorialPrecalculation {
 static_assert(is_modint_v<mod_t>);
 static inline std::vector<mod_t> iv, fct, fiv;
public:
 static void reset() { iv.clear(), fct.clear(), fiv.clear(); }
 static inline mod_t inv(int n) {
  assert(0 < n);
  if (int k= iv.size(); k <= n) {
   if (iv.resize(n + 1); !k) iv[1]= 1, k= 2;
   for (unsigned long long mod= mod_t::mod(), q; k <= n; ++k) q= (mod + k - 1) / k, iv[k]= iv[k * q - mod] * q;
  }
  return iv[n];
 }
 static inline mod_t fact(int n) {
  assert(0 <= n);
  if (int k= fct.size(); k <= n) {
   if (fct.resize(n + 1); !k) fct[0]= 1, k= 1;
   for (; k <= n; ++k) fct[k]= fct[k - 1] * k;
  }
  return fct[n];
 }
 static inline mod_t finv(int n) {
  assert(0 <= n);
  if (int k= fiv.size(); k <= n) {
   if (fiv.resize(n + 1); !k) fiv[0]= 1, k= 1;
   for (; k <= n; ++k) fiv[k]= fiv[k - 1] * inv(k);
  }
  return fiv[n];
 }
 static inline mod_t nPr(int n, int r) { return r < 0 || n < r ? mod_t(0) : fact(n) * finv(n - r); }
 // [x^r] (1 + x)^n
 static inline mod_t nCr(int n, int r) { return r < 0 || n < r ? mod_t(0) : fact(n) * finv(n - r) * finv(r); }
 // [x^r] (1 - x)^{-n}
 static inline mod_t nHr(int n, int r) { return !r ? mod_t(1) : nCr(n + r - 1, r); }
};