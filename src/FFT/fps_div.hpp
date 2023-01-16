#pragma once
#include <bits/stdc++.h>
#include "src/FFT/fps_inv.hpp"
namespace math_internal {
template <size_t LM, class mod_t> void div_base(const mod_t p[], int n, const mod_t q[], int l, mod_t r[], const mod_t iv[]) {
 static constexpr int t= nttarr_cat<mod_t, LM>, TH= (int[]){64, 64, 256, 256, 256, 256}[t];
 assert(n > 0), assert(((n & -n) == n)), assert(l > 0);
 const mod_t iv0= iv[0];
 const int m= min(TH, n);
 int i= 0;
 for (copy_n(p, m, r); i < m; r[i++]*= iv0)
  for (int j= min(i + 1, l); --j;) r[i]-= r[i - j] * q[j];
 using GNA1= GlobalNTTArray<mod_t, LM, 1>;
 using GNA2= GlobalNTTArray<mod_t, LM, 2>;
 using GNA3= GlobalNTTArray<mod_t, LM, 3>;
 auto gt1= GlobalNTTArray2D<mod_t, LM, 7, 1>::bf, gt2= GlobalNTTArray2D<mod_t, LM, 7, 2>::bf;
 int skip= (__builtin_ctz(n / i) + 2) % 3 + 1;
 for (int ed= (1 << skip) - 1; i < n; ed= 7) {
  mod_t* rr= r;
  const mod_t *qq= q, *pp= p;
  const int s= i, e= s << 1, ss= (l - 1) / s;
  GNA3::bf.set(iv, 0, s), GNA3::bf.zeros(s, e), GNA3::bf.dft(0, e);
  for (int k= 0, j; i < n && k < ed; ++k, i+= s, qq+= s, pp+= s) {
   if (j= min(e, l - k * s); j > 0) gt2[k].set(qq, 0, j), gt2[k].zeros(j, e), gt2[k].dft(0, e);
   gt1[k].set(rr, 0, s), gt1[k].zeros(s, e), gt1[k].dft(0, e);
   for (GNA2::bf.mul(gt1[k], gt2[0], 0, e), j= min(k, ss) + 1; --j;) GNA1::bf.mul(gt1[k - j], gt2[j], 0, e), GNA2::bf.add(GNA1::bf, 0, e);
   GNA2::bf.idft(0, e), GNA2::bf.zeros(0, s), GNA2::bf.get(rr, s, e);
   for (j= s; j < e; ++j) rr[j]-= pp[j];
   GNA2::bf.set(rr, s, e), GNA2::bf.dft(0, e), GNA2::bf.mul(GNA3::bf, 0, e), GNA2::bf.idft(0, e), GNA2::bf.get(rr, s, e);
   for (rr+= s, j= s; j--;) rr[j]= -rr[j];
  }
 }
}
template <size_t lnR, class mod_t, size_t LM= 1 << 22> void div_(const mod_t p[], int n, const mod_t q[], int l, mod_t r[]) {
 static constexpr size_t R= (1 << lnR) - 1, LM2= LM >> (lnR - 1);
 mod_t* iv= GlobalArray<mod_t, LM2, 2>::bf;
 using GNA1= GlobalNTTArray<mod_t, LM2, 1>;
 using GNA2= GlobalNTTArray<mod_t, LM2, 2>;
 using GNA3= GlobalNTTArray<mod_t, LM2, 3>;
 auto gt1= GlobalNTTArray2D<mod_t, LM2, R, 1>::bf, gt2= GlobalNTTArray2D<mod_t, LM2, R, 2>::bf;
 const int m= pw2(n) >> lnR, m2= m << 1, ed= (n - 1) / m, ss= (l - 1) / m;
 iv[0]= mod_t(1) / q[0], inv_base<LM2>(q, m, iv, 1, l), div_base<LM2>(p, m, q, l, r, iv), GNA3::bf.set(iv, 0, m), GNA3::bf.zeros(m, m2), GNA3::bf.dft(0, m2);
 for (int k= 0, i= m, j, o; k < ed; ++k, i+= m, q+= m, p+= m) {
  if (j= min(m2, l - k * m); j > 0) gt2[k].set(q, 0, j), gt2[k].zeros(j, m2), gt2[k].dft(0, m2);
  for (gt1[k].set(r, 0, m), gt1[k].zeros(m, m2), gt1[k].dft(0, m2), GNA2::bf.mul(gt1[k], gt2[0], 0, m2), j= min(k, ss) + 1; --j;) GNA1::bf.mul(gt1[k - j], gt2[j], 0, m2), GNA2::bf.add(GNA1::bf, 0, m2);
  for (GNA2::bf.idft(0, m2), GNA2::bf.zeros(0, m), GNA2::bf.get(r, m, m + (o= min(m, n - i))), j= o + m; j-- > m;) r[j]-= p[j];
  for (GNA2::bf.set(r, m, m + o), GNA2::bf.dft(0, m2), GNA2::bf.mul(GNA3::bf, 0, m2), GNA2::bf.idft(0, m2), GNA2::bf.get(r, m, m + o), r+= m, j= o; j--;) r[j]= -r[j];
 }
}
template <class mod_t, size_t LM= 1 << 22> vector<mod_t> div(const vector<mod_t>& p, const vector<mod_t>& q) {
 static constexpr int t= nttarr_cat<mod_t, LM>, TH= (int[]){120, 152, 361, 626, 359, 418}[t];
 mod_t *r= GlobalArray<mod_t, LM, 1>::bf, *qq= GlobalArray<mod_t, LM, 2>::bf;
 const int n= p.size(), l= q.size();
 assert(l > 0), assert(q[0] != mod_t(0));
 if (n > TH) {
  div_<3, mod_t, LM>(p.data(), n, q.data(), l, r);
 } else {
  const mod_t iv0= mod_t(1) / q[0];
  copy(p.begin(), p.end(), r), copy(q.begin(), q.end(), qq);
  for (int i= 0; i < n; r[i++]*= iv0)
   for (int j= min(i + 1, l); --j;) r[i]-= r[i - j] * qq[j];
 }
 return vector(r, r + n);
}
}
using math_internal::div;