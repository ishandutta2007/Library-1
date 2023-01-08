#pragma once
#include <bits/stdc++.h>
#include "src/FFT/fps_div.hpp"
template <class mod_t, std::size_t LM= 1 << 20> struct SubProductTree {
 using poly= std::vector<mod_t>;
 std::vector<mod_t> xs, all;
 int n, nn;
 std::vector<NTTArray<mod_t, LM, true>> p;
 mod_t *r= GlobalArray<mod_t, LM, 1>::bf;
 using GNA1= GlobalNTTArray<mod_t, LM, 1>;
 using GNA2= GlobalNTTArray<mod_t, LM, 2>;
 SubProductTree(const std::vector<mod_t> &xs_): xs(xs_), n(xs_.size()), nn(get_len(n)), p(nn << 1) {
  xs.resize(nn);
  for (int i= 0; i < nn; ++i) p[nn + i].resize(2), p[nn + i].set(0, 1), p[nn + i].set(1, -xs[i]);
  for (int i= nn; --i;) {
   int ls= i << 1, rs= i << 1 | 1, len= get_len(p[ls].size());
   p[ls].resize(len), p[rs].resize(len), p[ls].dft(0, len), p[rs].dft(0, len), p[i].resize(len), p[i].mul(p[ls], p[rs], 0, len), p[i].idft(0, len), p[i].resize(len + 1);
   if constexpr (!is_nttfriend<mod_t, LM>()) p[i].get(r, 1, len), p[i].set(r, 1, len);
   p[i].set(len, p[i].get(0) - 1), p[i].set(0, 1);
  }
  all.resize(n + 1), p[1].get(all.data(), 0, n + 1);
 }
 std::vector<mod_t> multi_eval(const poly &f) const {
  int m= f.size();
  if (m == 1) return std::vector<mod_t>(n, f[0]);
  auto q= div<mod_t, LM>(poly(f.rbegin(), f.rend()), all);
  if (m > nn) std::copy(q.end() - nn, q.end(), r);
  else std::copy(q.begin(), q.end(), r + nn - m), std::fill(r, r + nn - m, mod_t());
  for (int k= nn; k>>= 1;)
   for (int i= 0, k2= k << 1, o= nn / k; i < nn; i+= k2, o+= 2) GNA1::bf.set(r + i, 0, k2), GNA1::bf.dft(0, k2), GNA2::bf.mul(GNA1::bf, p[o | 1], 0, k2), GNA2::bf.idft(0, k2), GNA1::bf.mul(p[o], 0, k2), GNA1::bf.idft(0, k2), GNA2::bf.get(r + i - k, k, k2), GNA1::bf.get(r + i, k, k2);
  return std::vector<mod_t>(r, r + n);
 }
 poly interpolate(const std::vector<mod_t> &ys) {
  if (n == 1) assert(nn == 1);
  for (int i= n; i; --i) r[i - 1]= all[n - i] * i;
  auto q= multi_eval(poly(r, r + n));
  for (int i= n; i--;) r[i]= ys[i] / q[i];
  std::fill(r + n, r + nn, mod_t());
  for (int k= 1; k < nn; k<<= 1)
   for (int i= 0, o= nn / k, k2= k << 1; i < n; i+= k2, o+= 2) GNA1::bf.set(r + i, 0, k), GNA1::bf.zeros(k, k2), GNA2::bf.set(r + i + k, 0, k), GNA2::bf.zeros(k, k2), GNA1::bf.dft(0, k2), GNA2::bf.dft(0, k2), GNA1::bf.mul(p[o | 1], 0, k2), GNA2::bf.mul(p[o], 0, k2), GNA1::bf.add(GNA2::bf, 0, k2), GNA1::bf.idft(0, k2), GNA1::bf.get(r + i, 0, k2);
  return std::reverse(r, r + n), poly(r, r + n);
 }
};