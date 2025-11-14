#pragma once
#include <vector>
#include <cmath>
#include "src/FFT/NTT.hpp"
template <class mod_t, size_t LM= 1 << 22> std::vector<mod_t> convolve(const std::vector<mod_t>& p, const std::vector<mod_t>& q) {
 mod_t *pp= GlobalArray<mod_t, LM, 0>::bf, *qq= GlobalArray<mod_t, LM, 1>::bf, *rr= GlobalArray<mod_t, LM, 2>::bf;
 static constexpr int t= nttarr_cat<mod_t, LM>, TH= (int[]){70, 30, 70, 100, 135, 150}[t];
 auto f= [](int l) -> int {
  static constexpr double B[]= {(double[]){8.288, 5.418, 7.070, 9.676, 11.713, 13.374}[t], (double[]){8.252, 6.578, 9.283, 12.810, 13.853, 15.501}[t]};
  return std::round(std::pow(l, 0.535) * B[__builtin_ctz(l) & 1]);
 };
 const int n= p.size(), m= q.size(), sz= n + m - 1;
 if (!n || !m) return std::vector<mod_t>();
 if (std::min(n, m) < TH) {
  std::fill_n(rr, sz, mod_t()), std::copy(p.begin(), p.end(), pp), std::copy(q.begin(), q.end(), qq);
  for (int i= n; i--;)
   for (int j= m; j--;) rr[i + j]+= pp[i] * qq[j];
 } else {
  const int rl= pw2(sz), l= pw2(std::max(n, m)), fl= f(l);
  static constexpr size_t LM2= LM >> 3;
  static constexpr bool b= nttarr_cat<mod_t, LM2> < t;
  if (b || (l + fl < sz && sz <= (rl >> 3) * 5)) {
   using GNA1= GlobalNTTArray<mod_t, LM2, 1>;
   using GNA2= GlobalNTTArray<mod_t, LM2, 2>;
   auto gt1= GlobalNTTArray2D<mod_t, LM2, 16, 1>::bf, gt2= GlobalNTTArray2D<mod_t, LM2, 16, 2>::bf;
   const int l= rl >> 4, l2= l << 1, nn= (n + l - 1) / l, mm= (m + l - 1) / l, ss= nn + mm - 1;
   for (int i= 0, k= 0, s; k < n; ++i, k+= l) gt1[i].set(p.data() + k, 0, s= std::min(l, n - k)), gt1[i].zeros(s, l2), gt1[i].dft(0, l2);
   if (&p != &q)
    for (int i= 0, k= 0, s; k < m; ++i, k+= l) gt2[i].set(q.data() + k, 0, s= std::min(l, m - k)), gt2[i].zeros(s, l2), gt2[i].dft(0, l2);
   else
    for (int i= nn; i--;) gt2[i].subst(gt1[i], 0, l2);
   GNA2::bf.mul(gt1[0], gt2[0], 0, l2), GNA2::bf.idft(0, l2), GNA2::bf.get(rr, 0, l2);
   for (int i= 1, k= l, j, ed; i < ss; ++i, k+= l) {
    for (j= std::max(0, i - nn + 1), ed= std::min(mm - 1, i), GNA2::bf.mul(gt1[i - ed], gt2[ed], 0, l2); j < ed; ++j) GNA1::bf.mul(gt1[i - j], gt2[j], 0, l2), GNA2::bf.add(GNA1::bf, 0, l2);
    for (GNA2::bf.idft(0, l2), GNA2::bf.get(pp, 0, j= std::min(l, sz - k)); j--;) rr[k + j]+= pp[j];
    if (l < sz - k) GNA2::bf.get(rr + k, l, std::min(l2, sz - k));
   }
  } else {
   using GNA1= GlobalNTTArray<mod_t, LM, 1>;
   using GNA2= GlobalNTTArray<mod_t, LM, 2>;
   const int len= sz <= l + fl ? l : rl;
   if (GNA1::bf.set(p.data(), 0, n), GNA1::bf.zeros(n, len), GNA1::bf.dft(0, len); &p != &q) GNA2::bf.set(q.data(), 0, m), GNA2::bf.zeros(m, len), GNA2::bf.dft(0, len), GNA1::bf.mul(GNA2::bf, 0, len);
   else GNA1::bf.mul(GNA1::bf, 0, len);
   if (GNA1::bf.idft(0, len), GNA1::bf.get(rr, 0, std::min(sz, len)); len < sz) {
    std::copy(p.begin() + len - m + 1, p.end(), pp + len - m + 1), std::copy(q.begin() + len - n + 1, q.end(), qq + len - n + 1);
    for (int i= len, j; i < sz; rr[i - len]-= rr[i], ++i)
     for (rr[i]= mod_t(), j= i - m + 1; j < n; ++j) rr[i]+= pp[j] * qq[i - j];
   }
  }
 }
 return std::vector(rr, rr + sz);
}