#pragma once
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/mod_inv.hpp"
#include "src/Math/Factors.hpp"
/**
 * @title k乗根 ($\mathbb{F}_p$)
 * @category 数学
 * O( min(k,p)^(1/4) )
 * @see https://nyaannyaan.github.io/library/modulo/mod-kth-root.hpp
 */

// verify用
// https://yukicoder.me/problems/no/981 (厳しい制約のケースあり, sp judge)

// BEGIN CUT HERE
namespace kth_root_internal {
template <class Int, class mod_t>
mod_t peth_root(mod_t c, Int pi, int ei) {
  const Int p = mod_t::modulo();
  int t = 0;
  Int s = p - 1, pe = 1;
  while (s % pi == 0) s /= pi, ++t;
  for (int i = ei; i--;) pe *= pi;
  Int u = inv_mod(pe - s % pe, pe);
  mod_t ONE = 1, z = c.pow((s * u + 1) / pe), zpe = c.pow(s * u);
  if (zpe == ONE) return z;
  Int ptm1 = 1;
  for (int i = t; --i;) ptm1 *= pi;
  mod_t vs, base;
  for (mod_t v = 2;; v += ONE)
    if (vs = v.pow(s), base = vs.pow(ptm1); base != ONE) break;
  int size = 1 << std::__lg(int(std::sqrt(pi)) + 1), mask = size - 1,
      os[size + 1] = {};
  std::pair<mod_t, int> vec[size];
  mod_t x = 1, vspe = vs.pow(pe);
  for (int i = 0; i < size; ++i, x *= base) os[x.norm() & mask]++;
  for (int i = 1; i < size; ++i) os[i] += os[i - 1];
  x = 1, os[size] = size;
  for (int i = 0; i < size; ++i, x *= base) vec[--os[x.norm() & mask]] = {x, i};
  for (int vs_e = ei, td; zpe != ONE;) {
    mod_t tmp = zpe, base_zpe = mod_t(1) / zpe;
    for (td = 0; tmp != ONE; td++) tmp = tmp.pow(pi);
    for (int e = t - td; vs_e != e; vs_e++)
      vs = vs.pow(pi), vspe = vspe.pow(pi);
    for (int i = td; --i;) base_zpe = base_zpe.pow(pi);
    for (int tt = 0, upd = 1, n; upd; tt += size, base_zpe *= x)
      for (int m = (base_zpe.norm() & mask), i = os[m]; i < os[m + 1]; i++)
        if (base_zpe == vec[i].first) {
          if (n = vec[i].second - tt; n < 0) n += pi;
          z *= vs.pow(n), zpe *= vspe.pow(n), upd = false;
          break;
        }
  }
  return z;
}
template <class Int, class mod_t>
Int inner_kth_root(Int a, std::uint64_t k, Int p) {
  if (k == 0) return a == 1 ? a : -1;
  if (a <= 1 || k <= 1) return a;
  mod_t::set_mod(p);
  Int g = std::gcd(k, p - 1);
  mod_t ma = a;
  Int pp = (p - 1) / g, kk = (k / g) % pp;
  if (ma.pow(pp) != mod_t(1)) return -1;
  ma = ma.pow(inv_mod(kk, pp));
  for (auto [pi, ei] : Factors(g)) ma = peth_root<Int>(ma, pi, ei);
  return ma.val();
}
std::int64_t mod_kth_root(std::int64_t a, std::uint64_t k, std::int64_t p) {
  if (a %= p; p < INT_MAX)
    return inner_kth_root<int, RuntimeModInt<int, -2>>(a, k, p);
  else
    return inner_kth_root<std::int64_t, RuntimeModInt<Montgomery, -2>>(a, k, p);
}
}  // namespace kth_root_internal
using kth_root_internal::mod_kth_root;