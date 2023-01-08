#pragma once
#include <bits/stdc++.h>
#include "src/FFT/NTT.hpp"
class MultiVariateConvolution {
 const int n, k, m;
 std::vector<int> chi;
public:
 MultiVariateConvolution(): MultiVariateConvolution(std::vector<int>{}) {}
 MultiVariateConvolution(const std::vector<int> &dim): n(std::accumulate(dim.begin(), dim.end(), 1, std::multiplies<int>())), k(dim.size()), m(get_len(n) * 2), chi(n, 0) {
  for (int i= n; i--;)
   for (int den= 1, j= 0; j < k; ++j) chi[i]+= i / (den*= dim[j]);
  if (k)
   for (int i= n; i--;) chi[i]%= k;
 }
 int size() const { return n; }
 int dim() const { return k; }
 template <typename mod_t, std::size_t LM= 1 << 19, std::size_t LM2= 18> std::vector<mod_t> convolve(const std::vector<mod_t> &f, const std::vector<mod_t> &g) const {
  assert((int)f.size() == n), assert((int)g.size() == n);
  if (!k) return {f[0] * g[0]};
  mod_t *r= GlobalArray<mod_t, LM, 0>::bf;
  using GNA= GlobalNTTArray<mod_t, LM, 0>;
  auto gt0= GlobalNTTArray2D<mod_t, LM, LM2, 0>::bf, gt1= GlobalNTTArray2D<mod_t, LM, LM2, 1>::bf, gt2= GlobalNTTArray2D<mod_t, LM, LM2, 2>::bf;
  for (int i= k; i--;) gt0[i].zeros(0, m);
  for (int i= k; i--;) gt1[i].zeros(0, m);
  for (int i= k; i--;) gt2[i].zeros(0, m);
  for (int i= n; i--;) gt1[chi[i]].set(i, f[i]);
  for (int i= n; i--;) gt2[chi[i]].set(i, g[i]);
  for (int i= k; i--;) gt1[i].dft(0, m);
  for (int i= k; i--;) gt2[i].dft(0, m);
  for (int i= k, j, l; i--;)
   for (j= k; j--;) GNA::bf.mul(gt1[i], gt2[j], 0, m), gt0[l-= k & -((l= i + j) >= k)].add(GNA::bf, 0, m);
  for (int i= k; i--;) gt0[i].idft(0, m);
  for (int i= n; i--;) r[i]= gt0[chi[i]].get(i);
  return std::vector(r, r + n);
 }
};