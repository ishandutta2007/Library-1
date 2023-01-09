#pragma once
#include <bits/stdc++.h>
#include "src/FFT/fps_inv.hpp"
namespace math_internal {
template <int TH, int lnR, int skip, size_t LIM, class mod_t> void div_base_(const mod_t p[], int n, const mod_t q[], int l, mod_t r[], const mod_t iv[]) {
 static constexpr int LIM2= LIM >> (lnR - 1), R= (1 << lnR) - 1;
 using GNA1= GlobalNTTArray<mod_t, LIM2, 1>;
 using GNA2= GlobalNTTArray<mod_t, LIM2, 2>;
 using GNA3= GlobalNTTArray<mod_t, LIM2, 3>;
 auto gt1= GlobalNTTArray2D<mod_t, LIM2, R, 1>::bf;
 auto gt2= GlobalNTTArray2D<mod_t, LIM2, R, 2>::bf;
 assert(n > 0), assert(((n & -n) == n)), assert(l > 0);
 const mod_t iv0= iv[0];
 int i= 0;
 for (copy_n(p, TH, r); i < TH; r[i++]*= iv0)
  for (int j= min(i + 1, l); --j;) r[i]-= r[i - j] * q[j];
 for (int ed= (1 << skip) - 1; i < n; ed= R) {
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
   GNA2::bf.set(rr, s, e), GNA2::bf.dft(0, e), GNA2::bf.mul(GNA3::bf, 0, e);
   GNA2::bf.idft(0, e), GNA2::bf.get(rr, s, e);
   for (rr+= s, j= s; j--;) rr[j]= -rr[j];
  }
 }
}
template <size_t LIM, class mod_t> void div_base(const mod_t p[], int n, const mod_t q[], int l, mod_t r[], const mod_t iv[]) {
 static constexpr int t= nttarr_cat<mod_t, LIM>, TH= (int[]){64, 32, 128, 128, 128, 256}[t];
 assert(n > 0), assert(((n & -n) == n)), assert(l > 0);
 const mod_t iv0= iv[0];
 if (int i= 0; n > TH) {
  if constexpr (t == 0) {
   const int bn= __builtin_ctz(n) % 3;
   (bn == 0 ? div_base_<64, 3, 3, LIM, mod_t> : bn == 1 ? div_base_<32, 3, 2, LIM, mod_t> : div_base_<32, 3, 3, LIM, mod_t>)(p, n, q, l, r, iv);
  } else {
   const int bn= __builtin_ctz(n) % 6;
   if constexpr (t == 1) (bn & 1 ? div_base_<32, 2, 2, LIM, mod_t> : bn == 0 ? div_base_<32, 3, 1, LIM, mod_t> : bn == 2 ? div_base_<32, 3, 3, LIM, mod_t> : div_base_<32, 3, 2, LIM, mod_t>)(p, n, q, l, r, iv);
   else if constexpr (t == 5) (bn & 1 ? div_base_<128, 2, 2, LIM, mod_t> : bn == 0 ? div_base_<128, 3, 2, LIM, mod_t> : bn == 2 ? div_base_<256, 3, 3, LIM, mod_t> : div_base_<128, 3, 3, LIM, mod_t>)(p, n, q, l, r, iv);
   else (bn & 1 ? div_base_<128, 2, 2, LIM, mod_t> : bn == 0 ? div_base_<128, 3, 2, LIM, mod_t> : bn == 2 ? div_base_<128, 3, 1, LIM, mod_t> : div_base_<128, 3, 3, LIM, mod_t>)(p, n, q, l, r, iv);
  }
 } else
  for (copy_n(p, n, r); i < n; r[i++]*= iv0)
   for (int j= min(i + 1, l); --j;) r[i]-= r[i - j] * q[j];
}
template <size_t lnR, class mod_t, size_t LIM= 1 << 22> void div_(const mod_t p[], int n, const mod_t q[], int l, mod_t r[]) {
 static constexpr size_t R= (1 << lnR) - 1, LIM2= LIM >> (lnR - 1);
 mod_t* iv= GlobalArray<mod_t, LIM2, 2>::bf;
 using GNA1= GlobalNTTArray<mod_t, LIM2, 1>;
 using GNA2= GlobalNTTArray<mod_t, LIM2, 2>;
 using GNA3= GlobalNTTArray<mod_t, LIM2, 3>;
 auto gt1= GlobalNTTArray2D<mod_t, LIM2, R, 1>::bf, gt2= GlobalNTTArray2D<mod_t, LIM2, R, 2>::bf;
 const int m= pw2(n) >> lnR, m2= m << 1, ed= (n - 1) / m, ss= (l - 1) / m;
 iv[0]= mod_t(1) / q[0], inv_base<LIM2>(q, m, iv, 1, l), div_base<LIM2>(p, m, q, l, r, iv), GNA3::bf.set(iv, 0, m), GNA3::bf.zeros(m, m2), GNA3::bf.dft(0, m2);
 for (int k= 0, i= m, j, o; k < ed; ++k, i+= m, q+= m, p+= m) {
  if (j= min(m2, l - k * m); j > 0) gt2[k].set(q, 0, j), gt2[k].zeros(j, m2), gt2[k].dft(0, m2);
  for (gt1[k].set(r, 0, m), gt1[k].zeros(m, m2), gt1[k].dft(0, m2), GNA2::bf.mul(gt1[k], gt2[0], 0, m2), j= min(k, ss) + 1; --j;) GNA1::bf.mul(gt1[k - j], gt2[j], 0, m2), GNA2::bf.add(GNA1::bf, 0, m2);
  for (GNA2::bf.idft(0, m2), GNA2::bf.zeros(0, m), GNA2::bf.get(r, m, m + (o= min(m, n - i))), j= o + m; j-- > m;) r[j]-= p[j];
  for (GNA2::bf.set(r, m, m + o), GNA2::bf.dft(0, m2), GNA2::bf.mul(GNA3::bf, 0, m2), GNA2::bf.idft(0, m2), GNA2::bf.get(r, m, m + o), r+= m, j= o; j--;) r[j]= -r[j];
 }
}
template <class mod_t, size_t LIM= 1 << 22> vector<mod_t> div(const vector<mod_t>& p, const vector<mod_t>& q) {
 static constexpr int t= nttarr_cat<mod_t, LIM>, TH= (int[]){107, 75, 205, 350, 450, 600}[t];
 mod_t *r= GlobalArray<mod_t, LIM, 1>::bf, *qq= GlobalArray<mod_t, LIM, 2>::bf;
 const int n= p.size(), l= q.size();
 assert(l > 0), assert(q[0] != mod_t(0));
 if (n > TH) {
  const int m= pw2(n), l1= m >> 1, k= (n - l1 - 1) / (l1 >> 3), bl= __builtin_ctz(l1);
  int a= 5;
  if constexpr (!t) {
   a= bl < 8 ? 4 - (k & 1 && k > 2) : bl < 9 ? 4 : 4 + (k < (bl == 9 ? 1 : bl < 12 ? 3 : bl < 16 ? 4 : bl < 17 ? 5 : 6));
  } else if constexpr (t < 2) {
   if (bl & 1) {
    if (bl < 8) a= k && k != 2 ? 3 : 4;
    else if (k > 6) a= 3;
    else if (k > 5) a= 4;
    else if (!k) a= 5;
    else if (bl < 10) k & 1 ? 3 : 4;
    else if (bl < 12) a= k > 4 ? 3 : 4;
    else if (bl < 16) a= k > 4 ? 3 : k < (bl < 14 ? 2 : 3) ? 5 : 4;
    else a= k < 3 ? 5 : 4;
   } else {
    if (bl < 7) a= k & 1 && k > 2 ? 3 : 4;
    else if (bl < 17) a= 4;
    else a= k ? 4 : 5;
   }
  } else if constexpr (t < 3) {
   if (bl & 1) {
    if (bl < 8 || k > 6) a= 3;
    else if (bl < 10) a= k == 5 ? 3 : 4;
    else if (bl < 12) a= k < 2 ? 5 : k & 1 ? 3 : 4;
    else if (bl < 14) a= k < 2 ? 5 : k == 5 ? 3 : 4;
    else a= k < 3 ? 5 : 4;
   } else {
    if (bl < 9) a= k & 1 && k > 4 ? 3 : 4;
    else if (bl < 15) a= 4;
    else a= k ? 4 : 5;
   }
  } else if constexpr (t < 4) {
   if (bl & 1) {
    if (bl < 10) a= k & 1 || k > 4 ? 3 : 4;
    else if (k > 6) a= 3;
    else if (bl < 14) a= k < 2 ? 5 : k == 5 ? 3 : 4;
    else if (bl < 18) a= k < 3 ? 5 : 4;
    else a= k < 4 ? 5 : 4;
   } else {
    if (bl < 9) a= k & 1 && k > 2 ? 3 : 4;
    else if (bl < 15) a= 4;
    else a= k ? 4 : 5;
   }
  } else if constexpr (t < 5) {
   if (bl & 1) {
    if (bl < 10) a= k && k != 2 ? 3 : 4;
    else if (k > 6) a= 3;
    else if (bl < 14) a= k < 2 ? 5 : k == 5 ? 3 : 4;
    else if (bl < 18) a= k < 3 ? 5 : 4;
    else a= k < 4 ? 5 : 4;
   } else {
    if (bl < 9) a= 2;
    else if (bl < 15) a= 4;
    else a= k ? 4 : 5;
   }
  } else {
   if (bl & 1) {
    if (bl < 10) a= !k || k == 2 ? 4 : 3;
    else if (k > 6) a= 3;
    else if (bl < 12) a= k ? 4 : 5;
    else if (bl < 18) a= k < 3 ? 5 : 4;
    else a= k < 4 ? 5 : 4;
   } else {
    if (bl < 11) a= k > 6 ? 3 : 4;
    else if (bl < 15) a= 4;
    else a= k ? 4 : 5;
   }
  }
  (a == 2 ? div_<2, mod_t, LIM> : a == 3 ? div_<3, mod_t, LIM> : a == 4 ? div_<4, mod_t, LIM> : div_<5, mod_t, LIM>)(p.data(), n, q.data(), l, r);
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