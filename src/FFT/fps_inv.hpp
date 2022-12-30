#pragma once
#include <bits/stdc++.h>
#include "src/FFT/NTT.hpp"
namespace math_internal {
template <size_t LIM, class mod_t> inline void inv_base(const mod_t p[], int n, mod_t r[], int i= 1, int l= -1) {
  static constexpr int t= nttarr_cat<mod_t, LIM>, TH= (int[]){64, 32, 64, 128, 128, 256}[t];
  if (n <= i) return;
  if (l < 0) l= n;
  assert(((n & -n) == n)), assert(i && ((i & -i) == i));
  const mod_t miv= -r[0];
  if (; n > TH) {
    static constexpr int lnR= 2 + (!t), LIM2= LIM >> (lnR - 1), R= (1 << lnR) - 1;
    const auto [m, skip]= [&]() {
      if constexpr (!t) {
        const int bn= __builtin_ctz(n) % 3;
        return bn ? make_pair(64, bn) : make_pair(32, 1);
      } else return make_pair(TH, 1 + (__builtin_ctz(TH) & 1));
    }();
    for (fill_n(r + 1, m - 1, mod_t()); i < m; r[i++]*= miv)
      for (int j= min(i + 1, l); --j;) r[i]+= r[i - j] * p[j];
    using GNA1= GlobalNTTArray<mod_t, LIM2, 1>;
    using GNA2= GlobalNTTArray<mod_t, LIM2, 2>;
    auto gt1= GlobalNTTArray2D<mod_t, LIM2, R, 1>::bf, gt2= GlobalNTTArray2D<mod_t, LIM2, R, 2>::bf;
    for (int ed= (1 << skip) - 1; i < n; ed= R) {
      mod_t *rr= r;
      const mod_t *pp= p;
      const int s= i, e= s << 1, ss= (l - 1) / s;
      for (int k= 0, j; i < n && k < ed; ++k, i+= s, pp+= s) {
        if (j= min(e, l - k * s); j > 0) gt2[k].set(pp, 0, j), gt2[k].zeros(j, e), gt2[k].dft(0, e);
        for (gt1[k].set(rr, 0, s), gt1[k].zeros(s, e), gt1[k].dft(0, e), GNA2::bf.mul(gt1[k], gt2[0], 0, e), j= min(k, ss) + 1; --j;) GNA1::bf.mul(gt1[k - j], gt2[j], 0, e), GNA2::bf.add(GNA1::bf, 0, e);
        GNA2::bf.idft(0, e), GNA2::bf.zeros(0, s);
        if constexpr (!is_nttfriend<mod_t, LIM2>()) GNA2::bf.get(rr, s, e), GNA2::bf.set(rr, s, e);
        for (GNA2::bf.dft(0, e), GNA2::bf.mul(gt1[0], 0, e), GNA2::bf.idft(0, e), GNA2::bf.get(rr, s, e), rr+= s, j= s; j--;) rr[j]= -rr[j];
      }
    }
  } else
    for (fill_n(r + 1, n - 1, mod_t()); i < n; r[i++]*= miv)
      for (int j= min(i + 1, l); --j;) r[i]+= r[i - j] * p[j];
}
template <size_t lnR, class mod_t, size_t LIM= 1 << 22> void inv_(const mod_t p[], int n, mod_t r[]) {
  static constexpr size_t R= (1 << lnR) - 1, LIM2= LIM >> (lnR - 1);
  using GNA1= GlobalNTTArray<mod_t, LIM2, 1>;
  using GNA2= GlobalNTTArray<mod_t, LIM2, 2>;
  auto gt1= GlobalNTTArray2D<mod_t, LIM2, R, 1>::bf, gt2= GlobalNTTArray2D<mod_t, LIM2, R, 2>::bf;
  assert(n > 0), assert(p[0] != mod_t());
  const int m= get_len(n) >> lnR, m2= m << 1, ed= (n - 1) / m;
  inv_base<LIM2>(p, m, r);
  for (int k= 0, l; k < ed; p+= m) {
    for (gt2[k].set(p, 0, l= min(m2, n - m * k)), gt2[k].zeros(l, m2), gt2[k].dft(0, m2), gt1[k].set(r, 0, m), gt1[k].zeros(m, m2), gt1[k].dft(0, m2), GNA2::bf.mul(gt1[k], gt2[0], 0, m2), l= k; l--;) GNA1::bf.mul(gt1[l], gt2[k - l], 0, m2), GNA2::bf.add(GNA1::bf, 0, m2);
    GNA2::bf.idft(0, m2), GNA2::bf.zeros(0, m);
    if constexpr (!is_nttfriend<mod_t, LIM2>()) GNA2::bf.get(r, m, m2), GNA2::bf.set(r, m, m2);
    for (GNA2::bf.dft(0, m2), GNA2::bf.mul(gt1[0], 0, m2), GNA2::bf.idft(0, m2), GNA2::bf.get(r, m, m + (l= min(m, n - m * ++k))), r+= m; l--;) r[l]= -r[l];
  }
}
template <class mod_t, size_t LIM= 1 << 22> vector<mod_t> inv(const vector<mod_t> &p) {
  static constexpr int t= nttarr_cat<mod_t, LIM>, TH= (int[]){94, 54, 123, 222, 243, 354}[t];
  mod_t *pp= GlobalArray<mod_t, LIM, 1>::bf, *rr= GlobalArray<mod_t, LIM, 2>::bf;
  const int n= p.size();
  assert(n > 0), assert(p[0] != mod_t(0));
  copy(p.begin(), p.end(), pp);
  if (const mod_t miv= -(rr[0]= mod_t(1) / pp[0]); n > TH) {
    const int l= get_len(n), l1= l >> 1, k= (n - l1 - 1) / (l1 >> 3), bl= __builtin_ctz(l1);
    if constexpr (t != 0) {
      if (bl & 1) {
        static constexpr int BL= t == 5 ? 11 : 13;
        (k >= 6 ? inv_<1, mod_t, LIM> : !k && bl >= BL ? inv_<4, mod_t, LIM> : t == 2 && bl == 7 && k == 1 ? inv_<2, mod_t, LIM> : inv_<3, mod_t, LIM>)(pp, n, rr);
      } else {
        if (bl >= 10) (k >= 6 || k == 3 ? inv_<2, mod_t, LIM> : k == 5 ? inv_<3, mod_t, LIM> : inv_<4, mod_t, LIM>)(pp, n, rr);
        else if (bl == 6 || t == 4) (!k ? inv_<4, mod_t, LIM> : k == 1 ? inv_<3, mod_t, LIM> : inv_<2, mod_t, LIM>)(pp, n, rr);
        else (k >= 6 || (2 <= k && k < 4) ? inv_<2, mod_t, LIM> : k == 5 || (k == 1 && t != 1) ? inv_<3, mod_t, LIM> : inv_<4, mod_t, LIM>)(pp, n, rr);
      }
    } else (k & 1 ? inv_<3, mod_t, LIM> : inv_<4, mod_t, LIM>)(pp, n, rr);
  } else
    for (int j, i= 1; i < n; rr[i++]*= miv)
      for (rr[j= i]= mod_t(); j--;) rr[i]+= rr[j] * pp[i - j];
  return vector(rr, rr + n);
}
}
using math_internal::inv_base, math_internal::inv;