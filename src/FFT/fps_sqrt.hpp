#pragma once
#include <bits/stdc++.h>
#include "src/FFT/fps_inv.hpp"
#include "src/Math/mod_sqrt.hpp"
namespace math_internal {
template <size_t LM, class mod_t> void sqrt_base(const mod_t p[], int n, mod_t r[], int l, mod_t v[], mod_t iv[]) {
 static constexpr int t= nttarr_cat<mod_t, LM>, TH= (int[]){64, 64, 256, 256, 256, 256}[t];
 using GNA1= GlobalNTTArray<mod_t, LM, 1>;
 using GNA2= GlobalNTTArray<mod_t, LM, 2>;
 using GNA3= GlobalNTTArray<mod_t, LM, 3>;
 auto gt1= GlobalNTTArray2D<mod_t, LM, 7, 1>::bf;
 assert(n > 1);
 const int m= min(n, TH);
 const mod_t miv= mod_t(mod_t::mod() >> 1) / r[0];
 int i= 2;
 for ((r[1]-= p[1])*= miv; i < m; r[i]*= miv, ++i) {
  for (int j= (i + 1) / 2; --j;) r[i]+= r[j] * r[i - j];
  if (r[i]+= r[i]; !(i & 1)) r[i]+= r[i >> 1] * r[i >> 1];
  if (i < l) r[i]-= p[i];
 }
 if (i == n) return;
 int skip= (__builtin_ctz(n / i) + 2) % 3 + 1;
 v[0]= -r[0] - r[0], iv[0]= miv;
 for (int ed= (1 << skip) - 1, u= 1; i < n; ed= 7) {
  mod_t* rr= r;
  const mod_t* pp= p;
  const int s= i, e= s << 1;
  for (int j= u; j < s; ++j) v[j]= -r[j] - r[j];
  inv_base<LM>(v, s, iv, u), u= s;
  GNA1::bf.set(iv, 0, s), GNA1::bf.zeros(s, e), GNA1::bf.dft(0, e);
  for (int k= 0, j; i < n && k < ed; ++k, i+= s) {
   gt1[k].set(rr, 0, s), gt1[k].zeros(s, e), gt1[k].dft(0, e);
   for (GNA2::bf.zeros(0, e), j= k >> 1; j--;) GNA3::bf.add(gt1[j + 1], gt1[j], 0, s), GNA3::bf.dif(gt1[j + 1], gt1[j], s, e), GNA3::bf.mul(gt1[k - j], 0, e), GNA2::bf.add(GNA3::bf, 0, e);
   if (j= (k + 1) >> 1; k & 1) GNA3::bf.mul(gt1[j], gt1[k - j], 0, e), GNA2::bf.add(GNA3::bf, 0, s), GNA2::bf.dif(GNA3::bf, s, e);
   if (k) GNA2::bf.add(GNA2::bf, 0, e);
   GNA3::bf.mul(gt1[j], gt1[j], 0, e);
   k & 1 ? GNA2::bf.add(GNA3::bf, s, e) : GNA2::bf.dif(GNA3::bf, s, e);
   GNA2::bf.add(GNA3::bf, 0, s), GNA2::bf.idft(0, e), GNA2::bf.zeros(s, e), GNA2::bf.get(rr+= s, 0, s);
   if (j= min(s, l - s * k); j > 0)
    for (pp+= s; j--;) rr[j]-= pp[j];
   GNA2::bf.set(rr, 0, s), GNA2::bf.dft(0, e), GNA2::bf.mul(GNA1::bf, 0, e), GNA2::bf.idft(0, e), GNA2::bf.get(rr, 0, min(s, n - s * k));
  }
 }
}
template <class mod_t, size_t LM= 1 << 22> vector<mod_t> sqrt(const vector<mod_t>& p) {
 mod_t *r= GlobalArray<mod_t, LM, 1>::bf, *v= GlobalArray<mod_t, LM, 2>::bf, *iv= GlobalArray<mod_t, LM, 3>::bf;
 static constexpr size_t LM2= LM >> 2;
 int n= p.size(), cnt= 0;
 while (cnt < n && p[cnt] == mod_t()) cnt++;
 if (cnt == n) return p;
 if (cnt & 1) return {};  // no solution
 const int nn= n - (cnt >> 1), l= n - cnt;
 const mod_t* pp= p.data() + cnt;
 mod_t* rr= r + (cnt >> 1);
 fill_n(r, n, mod_t());
 if (rr[0]= mod_sqrt(pp[0].val(), mod_t::mod()); rr[0] * rr[0] != pp[0]) return {};  // no solution
 sqrt_base<LM2, mod_t>(pp, nn, rr, l, v, iv);
 return vector(r, r + n);
}
}
using math_internal::sqrt;