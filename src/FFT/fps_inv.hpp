#pragma once
#include <vector>
#include <cassert>
#include "src/FFT/NTT.hpp"
namespace math_internal {
template <u32 LM, class mod_t> inline void inv_base(const mod_t p[], int n, mod_t r[], int i= 1, int l= -1) {
 static constexpr int t= nttarr_cat<mod_t, LM>, TH= (int[]){64, 64, 128, 256, 512, 512}[t];
 if (n <= i) return;
 if (l < 0) l= n;
 assert(((n & -n) == n)), assert(i && ((i & -i) == i));
 const mod_t miv= -r[0];
 for (int j, m= min(n, TH); i < m; r[i++]*= miv)
  for (r[i]= mod_t(), j= min(i + 1, l); --j;) r[i]+= r[i - j] * p[j];
 static constexpr int lnR= 2 + (!t), R= (1 << lnR) - 1;
 using GNA1= GlobalNTTArray<mod_t, LM, 1>;
 using GNA2= GlobalNTTArray<mod_t, LM, 2>;
 for (auto gt1= GlobalNTTArray2D<mod_t, LM, R, 1>::bf, gt2= GlobalNTTArray2D<mod_t, LM, R, 2>::bf; i < n;) {
  mod_t* rr= r;
  const mod_t* pp= p;
  const int s= i, e= s << 1, ss= (l - 1) / s;
  for (int k= 0, j; i < n && k < R; ++k, i+= s, pp+= s) {
   if (j= min(e, l - k * s); j > 0) gt2[k].set(pp, 0, j), gt2[k].zeros(j, e), gt2[k].dft(0, e);
   for (gt1[k].set(rr, 0, s), gt1[k].zeros(s, e), gt1[k].dft(0, e), GNA2::bf.mul(gt1[k], gt2[0], 0, e), j= min(k, ss) + 1; --j;) GNA1::bf.mul(gt1[k - j], gt2[j], 0, e), GNA2::bf.add(GNA1::bf, 0, e);
   GNA2::bf.idft(0, e), GNA2::bf.zeros(0, s);
   if constexpr (!is_nttfriend<mod_t, LM>()) GNA2::bf.get(rr, s, e), GNA2::bf.set(rr, s, e);
   for (GNA2::bf.dft(0, e), GNA2::bf.mul(gt1[0], 0, e), GNA2::bf.idft(0, e), GNA2::bf.get(rr, s, e), rr+= j= s; j--;) rr[j]= -rr[j];
  }
 }
}
template <u32 lnR, class mod_t, u32 LM= 1 << 22> void inv_(const mod_t p[], int n, mod_t r[]) {
 static constexpr u32 R= (1 << lnR) - 1, LM2= LM >> (lnR - 1);
 using GNA1= GlobalNTTArray<mod_t, LM2, 1>;
 using GNA2= GlobalNTTArray<mod_t, LM2, 2>;
 auto gt1= GlobalNTTArray2D<mod_t, LM2, R, 1>::bf, gt2= GlobalNTTArray2D<mod_t, LM2, R, 2>::bf;
 assert(n > 0), assert(p[0] != mod_t());
 const int m= pw2(n) >> lnR, m2= m << 1, ed= (n - 1) / m;
 inv_base<LM2>(p, m, r);
 for (int k= 0, l; k < ed; p+= m) {
  for (gt2[k].set(p, 0, l= min(m2, n - m * k)), gt2[k].zeros(l, m2), gt2[k].dft(0, m2), gt1[k].set(r, 0, m), gt1[k].zeros(m, m2), gt1[k].dft(0, m2), GNA2::bf.mul(gt1[k], gt2[0], 0, m2), l= k; l--;) GNA1::bf.mul(gt1[l], gt2[k - l], 0, m2), GNA2::bf.add(GNA1::bf, 0, m2);
  GNA2::bf.idft(0, m2), GNA2::bf.zeros(0, m);
  if constexpr (!is_nttfriend<mod_t, LM>()) GNA2::bf.get(r, m, m2), GNA2::bf.set(r, m, m2);
  for (GNA2::bf.dft(0, m2), GNA2::bf.mul(gt1[0], 0, m2), GNA2::bf.idft(0, m2), GNA2::bf.get(r, m, m + (l= min(m, n - m * ++k))), r+= m; l--;) r[l]= -r[l];
 }
}
template <class mod_t, u32 LM= 1 << 22> vector<mod_t> inv(const vector<mod_t>& p) {
 static constexpr int t= nttarr_cat<mod_t, LM>, TH= (int[]){234, 106, 280, 458, 603, 861}[t];
 mod_t *pp= GlobalArray<mod_t, LM, 1>::bf, *r= GlobalArray<mod_t, LM, 2>::bf;
 const int n= p.size();
 copy_n(p.begin(), n, pp), assert(n > 0), assert(p[0] != mod_t());
 if (const mod_t miv= -(r[0]= mod_t(1) / p[0]); n > TH) {
  const int l= pw2(n), l1= l >> 1, k= (n - l1 - 1) / (l1 >> 3), bl= __builtin_ctz(l1);
  int a= 4;
  if constexpr (!t) a= bl < 8 ? k > 5 ? 1 : 3 : bl < 9 ? k & 1 ? 3 : 4 : bl < 10 ? k & 1 && k > 4 ? 3 : 4 : bl < 11 ? k > 6 ? 3 : 4 : 4;
  else if constexpr (t < 2) a= bl < 7 ? 2 : bl < 9 ? k ? 3 : 4 : k & 1 ? 3 : 4;
  else if constexpr (t < 3) a= bl < 9 ? k > 5 ? 1 : k ? 3 : 4 : k & 1 ? 3 : 4;
  else if constexpr (t < 4) a= bl < 9 ? 1 : bl < 10 ? k > 5 ? 1 : !k ? 4 : k & 2 ? 2 : 3 : k & 1 ? 3 : 4;
  else if constexpr (t < 5) a= bl < 10 ? k & 2 ? 2 : 3 : k & 1 ? 3 : 4;
  else a= bl < 10 ? 1 : bl < 11 ? k > 5 ? 1 : !k ? 4 : k & 2 ? 2 : 3 : k & 1 ? 3 : 4;
  (a < 2 ? inv_<1, mod_t, LM> : a < 3 ? inv_<2, mod_t, LM> : a < 4 ? inv_<3, mod_t, LM> : inv_<4, mod_t, LM>)(pp, n, r);
 } else
  for (int j, i= 1; i < n; r[i++]*= miv)
   for (r[j= i]= mod_t(); j--;) r[i]+= r[j] * pp[i - j];
 return vector(r, r + n);
}
}
using math_internal::inv_base, math_internal::inv;