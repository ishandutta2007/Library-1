#pragma once
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/FFT/fps_div.hpp"
namespace math_internal {
template <class mod_t> vector<mod_t> deriv(const vector<mod_t> &p) {
 vector<mod_t> ret(p.size() - 1);
 for (int i= p.size(); --i;) ret[i - 1]= p[i] * i;
 return ret;
}
template <class mod_t, size_t LM= 1 << 22> vector<mod_t> integ(const vector<mod_t> &p) {
 vector<mod_t> ret(p.size() + 1, mod_t());
 for (int i= p.size(); i; --i) ret[i]= p[i - 1] * get_inv<mod_t, LM>(i);
 return ret;
}
template <class mod_t, size_t LM= 1 << 22> vector<mod_t> log(const vector<mod_t> &p) {
 assert(p[0] == mod_t(1));
 return integ<mod_t, LM>(div<mod_t, LM>(deriv(p), p));
}
template <class mod_t, size_t LM= 1 << 22> vector<mod_t> exp(const vector<mod_t> &p) {
 static constexpr int LM2= LM * 2 / 15;
 static constexpr int TH= 64 << ((!is_nttfriend<mod_t, LM>()) << 1);
 mod_t *dp= GlobalArray<mod_t, LM, 1>::bf, *rr= GlobalArray<mod_t, LM, 2>::bf, *g= GlobalArray<mod_t, LM2, 0>::bf;
 using GNA1= GlobalNTTArray<mod_t, LM2, 1>;
 using GNA2= GlobalNTTArray<mod_t, LM2, 2>;
 auto gt1= GlobalNTTArray2D<mod_t, LM2, 16, 1>::bf, gt2= GlobalNTTArray2D<mod_t, LM2, 16, 2>::bf;
 const int n= p.size(), m= pw2(n);
 assert(n > 0), assert(p[0] == mod_t());
 copy(p.begin(), p.end(), dp);
 for (int i= n; --i;) dp[i]*= i;
 fill_n(rr, n, mod_t()), rr[0]= 1;
 for (int r= m, d= 0, R, k, i; r > TH; d+= k) {
  k= (r/= (R= pw2(__builtin_ctz(r) + 1) >> 1)) << 1;
  for (i= min(R - 1, (n - 1) / r); i--;) gt1[i].set(dp + i * r - d, d, d + k), gt1[i].dft(d, d + k);
 }
 auto rec= [&](auto f, int l, int r, int d) -> void {
  if (int i= l | (!l), ed= min(r, n), j; r - l > TH) {
   int R= pw2(__builtin_ctz(r - l) + 1) >> 1, len= (r - l) / R, k= len << 1;
   for (i= 0, ed= min(R, (n - l + len - 1) / len);; i++) {
    if (mod_t *ret= rr + l + i * len, *bf= g + d + len; i) {
     for (GNA1::bf.zeros(d, d + k), j= i; j--;) GNA2::bf.mul(gt2[j], gt1[i - j - 1], d, d + k), GNA1::bf.add(GNA2::bf, d, d + k);
     GNA1::bf.idft(d, d + k), GNA1::bf.get(g, d + len, d + k);
     for (int t= len; t--;) ret[t]+= bf[t];
    }
    if (f(f, l + i * len, l + (i + 1) * len, d + k); i == ed - 1) break;
    gt2[i].set(rr + l + i * len - d, d, d + len);
    gt2[i].zeros(d + len, d + k), gt2[i].dft(d, d + k);
   }
  } else
   for (; i < ed; rr[i]*= get_inv<mod_t, LM>(i), ++i)
    for (j= l; j < i; j++) rr[i]+= rr[j] * dp[i - j];
 };
 return rec(rec, 0, m, 0), vector(rr, rr + n);
}
template <class mod_t, size_t LM= 1 << 22> vector<mod_t> pow(const vector<mod_t> &p, uint64_t k) {
 mod_t *g= GlobalArray<mod_t, LM, 4>::bf;
 const mod_t MK(k);
 int n= p.size(), cnt= 0;
 if (g[0]= 1; k) {
  while (cnt < n && p[cnt] == mod_t()) cnt++;
  const __int128_t ofs= (__int128_t)k * cnt, sz= n - ofs;
  if (sz <= 0) return vector<mod_t>(n, mod_t());
  const mod_t p0= p[cnt], iv= mod_t(1) / p0, pk= p0.pow(k);
  for (int i= sz; --i;) g[i]= p[i + cnt] * iv;
  auto q= log<mod_t, LM>(vector<mod_t>(g, g + sz));
  for (int i= sz; --i;) q[i]*= MK;
  copy_n(exp<mod_t, LM>(q).begin(), (int)sz, g + ofs);
  fill_n(g, (int)ofs, mod_t());
  for (int i= sz; i--;) g[i + ofs]*= pk;
 } else fill_n(g + 1, n - 1, mod_t());
 return vector(g, g + n);
}
}  // namespace math_internal
using math_internal::deriv, math_internal::integ, math_internal::log, math_internal::exp, math_internal::pow;