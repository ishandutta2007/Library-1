#pragma once
#include <bits/stdc++.h>
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
namespace math_internal {
template <class Int, class mod_pro_t>
inline Int peth_root(Int c, Int pi, int ei, const mod_pro_t &md) {
  const Int p = md.modulo();
  int t = 0;
  Int s = p - 1, pe = 1;
  while (s % pi == 0) s /= pi, ++t;
  for (int i = ei; i--;) pe *= pi;
  Int u = mod_inv(pe - s % pe, pe), ONE = md.set(1),
      z = pow(c, (s * u + 1) / pe, md), zpe = md.norm(pow(c, s * u, md));
  if (zpe == ONE) return z;
  Int ptm1 = 1, vs = 0, bs = 0;
  for (int i = t; --i;) ptm1 *= pi;
  for (Int v = md.set(2);; v = md.plus(v, ONE))
    if (vs = pow(v, s, md), bs = md.norm(pow(vs, ptm1, md)); bs != ONE) break;
  int size = 1 << __lg(int(sqrt(pi)) + 1), mask = size - 1, vsc[size];
  std::vector<int> os(size + 1);
  Int vf[size];
  Int x = ONE, vspe = pow(vs, pe, md);
  for (int i = 0; i < size; i++, x = md.mul(x, bs)) os[md.norm(x) & mask]++;
  for (int i = 1; i < size; i++) os[i] += os[i - 1];
  x = ONE, os[size] = size;
  for (int i = 0, j = 0; i < size; i++, x = md.mul(x, bs))
    vf[j = --os[md.norm(x) & mask]] = md.norm(x), vsc[j] = i;
  for (int vs_e = ei, td = 0, n = 0; zpe != ONE; z = md.mul(z, pow(vs, n, md)),
           zpe = md.norm(md.mul(zpe, pow(vspe, n, md)))) {
    for (u = zpe, td = 0; u != ONE; td++) u = md.norm(pow(bs = u, pi, md));
    for (int e = t - td; vs_e != e; vs_e++)
      vs = pow(vs, pi, md), vspe = pow(vspe, pi, md);
    for (int tt = 0, upd = 1; upd; tt += size, bs = md.mul(bs, x))
      for (int m = (md.norm(bs) & mask), i = os[m]; i < os[m + 1]; i++)
        if (md.norm(bs) == vf[i]) {
          if (upd = false, n = tt - vsc[i]; n < 0) n += pi;
          break;
        }
  }
  return z;
}
template <class Int, class mod_pro_t>
Int inner_kth_root(Int a, u64 k, Int p) {
  if (k == 0) return a == 1 ? a : -1;
  if (a <= 1 || k <= 1) return a;
  const mod_pro_t md(p);
  Int g = gcd(k, p - 1), pp = (p - 1) / g, kk = (k / g) % pp;
  if (a = md.set(a); md.norm(pow(a, pp, md)) != md.set(1)) return -1;
  a = pow(a, mod_inv(kk, pp), md);
  for (auto [pi, ei] : Factors(g)) a = peth_root<Int>(a, pi, ei, md);
  return md.get(a);
}
int64_t mod_kth_root(int64_t a, u64 k, int64_t p) {
  assert(p > 0), assert(a >= 0), assert(is_prime(p)), a %= p;
  if (p < INT_MAX) return inner_kth_root<int, MIntPro_Na<u32>>(a, k, p);
  return inner_kth_root<int64_t, MIntPro_Montg>(a, k, p);
}
}  // namespace math_internal
using math_internal::mod_kth_root;