#pragma once
#include <bits/stdc++.h>
#include "src/FFT/NTT.hpp"
template <class mod_t, std::size_t LM= 1 << 24> std::vector<mod_t> sample_points_shift(const std::vector<mod_t>& y, mod_t c, int m= 1) {
 assert(m <= mod_t::mod()), assert(y.size() <= mod_t::mod());
 static constexpr int TH= (int[]){45, 32, 75, 130, 180, 260}[nttarr_cat<mod_t, LM>];
 if (m == 0) return {};
 std::uint64_t c_64= c.val(), nc1= (c + (m - 1)).val();
 std::uint32_t k= y.size(), d= k - 1, i= d, e;
 if (c_64 + m <= k) return std::vector<mod_t>(y.begin() + c_64, y.begin() + c_64 + m);
 mod_t *x= GlobalArray<mod_t, LM, 0>::bf, *p= GlobalArray<mod_t, LM, 1>::bf, *bf;
 for (x[d]= 1; i; i--) x[i - 1]= x[i] * i;
 mod_t t= mod_t(1) / (x[0] * x[0]);
 for (i= d / 2 + 1; i--;) x[i]= x[d - i]= x[i] * x[d - i] * t;
 for (i= k; i--;) x[i]*= y[i];
 for (i= 1; i < k; i+= 2) x[d - i]= -x[d - i];
 auto f= [&](mod_t a, int n, mod_t ret[]) {
  using GNA1= GlobalNTTArray<mod_t, LM, 1>;
  using GNA2= GlobalNTTArray<mod_t, LM, 2>;
  mod_t* q= GlobalArray<mod_t, LM, 2>::bf;
  for (e= d + n, i= 0, t= a - d; i < e; ++i, t+= 1) ret[i]= t;
  std::partial_sum(ret, ret + e, q, std::multiplies<>());
  for (t= mod_t(1) / q[e - 1]; --i;) q[i]= t * q[i - 1], t*= ret[i];
  if (q[0]= t; k >= TH && n >= TH) {
   const int len= pw2(e + (d > 0));
   GNA1::bf.set(x, 0, k), GNA1::bf.zeros(k, len), GNA1::bf.dft(0, len), GNA2::bf.set(q, 0, e), GNA2::bf.zeros(e, len), GNA2::bf.dft(0, len), GNA1::bf.mul(GNA2::bf, 0, len), GNA1::bf.idft(0, len), GNA1::bf.get(ret - d, d, e);
  } else
   for (std::fill_n(ret, n, mod_t()), i= k; i--;)
    for (int b= d - i, j= n; j--;) ret[j]+= x[i] * q[j + b];
  for (t= a, i= k; --i;) t*= a - i;
  for (; i < n; i++) ret[i]*= t, t*= (a + (i + 1)) * q[i];
  return ret + n;
 };
 if (c_64 < k) {
  if (bf= std::copy_n(y.begin() + c_64, k - c_64, p); nc1 < k) std::copy_n(y.begin(), nc1 + 1, f(k, mod_t::mod() - k, bf));
  else f(k, c_64 + m - k, bf);
 } else if (nc1 < c_64) {
  if (bf= f(c, (-c).val(), p); nc1 < k) std::copy_n(y.begin(), nc1 + 1, bf);
  else f(k, nc1 + 1 - k, std::copy_n(y.begin(), k, bf));
 } else f(c, m, p);
 return std::vector(p, p + m);
}